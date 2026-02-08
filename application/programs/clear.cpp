
#include "clear.h"

#include "library/core/helpers.h"

namespace GodotObjectCompiler {

  Ref<ProgramError> Clear::run(ApplicationContext& p_context) {
    ClearCache clear_cache;
    if (Ref<ProgramError> clear_cache_error = clear_cache.run(p_context); clear_cache_error != ProgramError::OK) {
      return clear_cache_error;
    }

    ClearGenerated clear_generated;
    if (Ref<ProgramError> clear_generated_error = clear_generated.run(p_context);
        clear_generated_error != ProgramError::OK) {
      return clear_generated_error;
    }

    return ProgramError::OK;
  }

  Ref<ProgramError> ClearGenerated::run(ApplicationContext& p_context) {
    for (const String& entry : directory_entries(p_context.paths_generated)) {
      if (!remove(entry)) {
        make_ref<Error>(ERROR, format("Failed to remove \"%s\"", entry.c_str()));
      }
    }

    return ProgramError::OK;
  }

  Ref<ProgramError> ClearCache::run(ApplicationContext& p_context) {
    for (const String& entry : directory_entries(p_context.paths_cache)) {
      if (!remove(entry)) {
        make_ref<Error>(ERROR, format("Failed to remove \"%s\"", entry.c_str()));
      }
    }

    return ProgramError::OK;
  }

}  // namespace GodotObjectCompiler