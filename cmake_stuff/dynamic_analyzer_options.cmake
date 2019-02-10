set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fdiagnostics-show-category=name")

if(ANALYZE_MEMORY)
  set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fsanitize=memory")
  set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fsanitize-memory-track-origins=2")
  set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fno-omit-frame-pointer")
  # NOTE: tail call elimination -fno-optimize-sibling-calls
elseif(ANALYZE_ADDRESS)
  set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fsanitize=address")
  set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fno-omit-frame-pointer")
elseif(ANALYZE_THREAD)
  set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fsanitize=thread")
endif()

if(ANALYZE_UNDEFINED)
  set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fsanitize=undefined-trap")
  set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fsanitize-undefined-trap-on-error")
  set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fno-sanitize-recover")
endif()

if(ANALYZE_DATAFLOW)
  set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fsanitize=dataflow")
endif()
