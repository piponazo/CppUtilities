/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/
#include "precomp.hpp"

/* default alignment for dynamic data strucutures, resided in storages. */
#define  CV_STRUCT_ALIGN    ((int)sizeof(double))

/* default storage block size */
#define  CV_STORAGE_BLOCK_SIZE   ((1<<16) - 128)

#define ICV_FREE_PTR(storage)  \
    ((schar*)(storage)->top + (storage)->block_size - (storage)->free_space)

#define ICV_ALIGNED_SEQ_BLOCK_SIZE  \
    (int)cvAlign(sizeof(CvSeqBlock), CV_STRUCT_ALIGN)

CV_INLINE int
cvAlignLeft( int size, int align )
{
    return size & -align;
}

#define CV_GET_LAST_ELEM( seq, block ) \
    ((block)->data + ((block)->count - 1)*((seq)->elem_size))

#define CV_SWAP_ELEMS(a,b,elem_size)  \
{                                     \
    int k;                            \
    for( k = 0; k < elem_size; k++ )  \
    {                                 \
        char t0 = (a)[k];             \
        char t1 = (b)[k];             \
        (a)[k] = t1;                  \
        (b)[k] = t0;                  \
    }                                 \
}

#define ICV_SHIFT_TAB_MAX 32
static const schar icvPower2ShiftTab[] =
{
    0, 1, -1, 2, -1, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1, 4,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5
};

/****************************************************************************************\
*            Functions for manipulating memory storage - list of memory blocks           *
\****************************************************************************************/

/* Initialize allocated storage: */
static void
icvInitMemStorage( CvMemStorage* storage, int block_size )
{
    if( !storage )
        CV_Error( CV_StsNullPtr, "" );

    if( block_size <= 0 )
        block_size = CV_STORAGE_BLOCK_SIZE;

    block_size = cvAlign( block_size, CV_STRUCT_ALIGN );
    assert( sizeof(CvMemBlock) % CV_STRUCT_ALIGN == 0 );

    memset( storage, 0, sizeof( *storage ));
    storage->signature = CV_STORAGE_MAGIC_VAL;
    storage->block_size = block_size;
}


/* Create root memory storage: */
CV_IMPL CvMemStorage*
cvCreateMemStorage( int block_size )
{
    CvMemStorage* storage = (CvMemStorage *)cvAlloc( sizeof( CvMemStorage ));
    icvInitMemStorage( storage, block_size );
    return storage;
}


/* Create child memory storage: */
CV_IMPL CvMemStorage *
cvCreateChildMemStorage( CvMemStorage * parent )
{
    if( !parent )
        CV_Error( CV_StsNullPtr, "" );

    CvMemStorage* storage = cvCreateMemStorage(parent->block_size);
    storage->parent = parent;

    return storage;
}


/* Release all blocks of the storage (or return them to parent, if any): */
static void
icvDestroyMemStorage( CvMemStorage* storage )
{
    int k = 0;

    CvMemBlock *block;
    CvMemBlock *dst_top = 0;

    if( !storage )
        CV_Error( CV_StsNullPtr, "" );

    if( storage->parent )
        dst_top = storage->parent->top;

    for( block = storage->bottom; block != 0; k++ )
    {
        CvMemBlock *temp = block;

        block = block->next;
        if( storage->parent )
        {
            if( dst_top )
            {
                temp->prev = dst_top;
                temp->next = dst_top->next;
                if( temp->next )
                    temp->next->prev = temp;
                dst_top = dst_top->next = temp;
            }
            else
            {
                dst_top = storage->parent->bottom = storage->parent->top = temp;
                temp->prev = temp->next = 0;
                storage->free_space = storage->block_size - sizeof( *temp );
            }
        }
        else
        {
            cvFree( &temp );
        }
    }

    storage->top = storage->bottom = 0;
    storage->free_space = 0;
}


/* Release memory storage: */
CV_IMPL void
cvReleaseMemStorage( CvMemStorage** storage )
{
    if( !storage )
        CV_Error( CV_StsNullPtr, "" );

    CvMemStorage* st = *storage;
    *storage = 0;
    if( st )
    {
        icvDestroyMemStorage( st );
        cvFree( &st );
    }
}


/* Clears memory storage (return blocks to the parent, if any): */
CV_IMPL void
cvClearMemStorage( CvMemStorage * storage )
{
    if( !storage )
        CV_Error( CV_StsNullPtr, "" );

    if( storage->parent )
        icvDestroyMemStorage( storage );
    else
    {
        storage->top = storage->bottom;
        storage->free_space = storage->bottom ? storage->block_size - sizeof(CvMemBlock) : 0;
    }
}


/* Moves stack pointer to next block.
   If no blocks, allocate new one and link it to the storage: */
static void
icvGoNextMemBlock( CvMemStorage * storage )
{
    if( !storage )
        CV_Error( CV_StsNullPtr, "" );

    if( !storage->top || !storage->top->next )
    {
        CvMemBlock *block;

        if( !(storage->parent) )
        {
            block = (CvMemBlock *)cvAlloc( storage->block_size );
        }
        else
        {
            CvMemStorage *parent = storage->parent;
            CvMemStoragePos parent_pos;

            cvSaveMemStoragePos( parent, &parent_pos );
            icvGoNextMemBlock( parent );

            block = parent->top;
            cvRestoreMemStoragePos( parent, &parent_pos );

            if( block == parent->top )  /* the single allocated block */
            {
                assert( parent->bottom == block );
                parent->top = parent->bottom = 0;
                parent->free_space = 0;
            }
            else
            {
                /* cut the block from the parent's list of blocks */
                parent->top->next = block->next;
                if( block->next )
                    block->next->prev = parent->top;
            }
        }

        /* link block */
        block->next = 0;
        block->prev = storage->top;

        if( storage->top )
            storage->top->next = block;
        else
            storage->top = storage->bottom = block;
    }

    if( storage->top->next )
        storage->top = storage->top->next;
    storage->free_space = storage->block_size - sizeof(CvMemBlock);
    assert( storage->free_space % CV_STRUCT_ALIGN == 0 );
}


/* Remember memory storage position: */
CV_IMPL void
cvSaveMemStoragePos( const CvMemStorage * storage, CvMemStoragePos * pos )
{
    if( !storage || !pos )
        CV_Error( CV_StsNullPtr, "" );

    pos->top = storage->top;
    pos->free_space = storage->free_space;
}


/* Restore memory storage position: */
CV_IMPL void
cvRestoreMemStoragePos( CvMemStorage * storage, CvMemStoragePos * pos )
{
    if( !storage || !pos )
        CV_Error( CV_StsNullPtr, "" );
    if( pos->free_space > storage->block_size )
        CV_Error( CV_StsBadSize, "" );

    /*
    // this breaks icvGoNextMemBlock, so comment it off for now
    if( storage->parent && (!pos->top || pos->top->next) )
    {
        CvMemBlock* save_bottom;
        if( !pos->top )
            save_bottom = 0;
        else
        {
            save_bottom = storage->bottom;
            storage->bottom = pos->top->next;
            pos->top->next = 0;
            storage->bottom->prev = 0;
        }
        icvDestroyMemStorage( storage );
        storage->bottom = save_bottom;
    }*/

    storage->top = pos->top;
    storage->free_space = pos->free_space;

    if( !storage->top )
    {
        storage->top = storage->bottom;
        storage->free_space = storage->top ? storage->block_size - sizeof(CvMemBlock) : 0;
    }
}


/* Allocate continuous buffer of the specified size in the storage: */
CV_IMPL void*
cvMemStorageAlloc( CvMemStorage* storage, size_t size )
{
    schar *ptr = 0;
    if( !storage )
        CV_Error( CV_StsNullPtr, "NULL storage pointer" );

    if( size > INT_MAX )
        CV_Error( CV_StsOutOfRange, "Too large memory block is requested" );

    assert( storage->free_space % CV_STRUCT_ALIGN == 0 );

    if( (size_t)storage->free_space < size )
    {
        size_t max_free_space = cvAlignLeft(storage->block_size - sizeof(CvMemBlock), CV_STRUCT_ALIGN);
        if( max_free_space < size )
            CV_Error( CV_StsOutOfRange, "requested size is negative or too big" );

        icvGoNextMemBlock( storage );
    }

    ptr = ICV_FREE_PTR(storage);
    assert( (size_t)ptr % CV_STRUCT_ALIGN == 0 );
    storage->free_space = cvAlignLeft(storage->free_space - (int)size, CV_STRUCT_ALIGN );

    return ptr;
}


/****************************************************************************************\
*                               Sequence implementation                                  *
\****************************************************************************************/

/****************************************************************************************\
*                             Sequence Writer implementation                             *
\****************************************************************************************/

/****************************************************************************************\
*                               Sequence Reader implementation                           *
\****************************************************************************************/

/****************************************************************************************\
*                                      Set implementation                                *
\****************************************************************************************/


/****************************************************************************************\
*                                 Graph  implementation                                  *
\****************************************************************************************/


/****************************************************************************************\
*                                 Working with sequence tree                             *
\****************************************************************************************/


/* End of file. */
