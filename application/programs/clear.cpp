
#include "clear.h"

#include "library/core/helpers.h"

namespace GodotObjectCompiler {

  Ref<ProgramError> Clear::run(ApplicationContext& context) {
    ClearCache clear_cache;
    if (Ref<ProgramError> clear_cache_error = clear_cache.run(context); clear_cache_error != ProgramError::OK) {
      return clear_cache_error;
    }

    ClearGenerated clear_generated;
    if (Ref<ProgramError> clear_generated_error = clear_generated.run(context);
        clear_generated_error != ProgramError::OK) {
      return clear_generated_error;
    }

    return ProgramError::OK;
  }

  Ref<ProgramError> ClearGenerated::run(ApplicationContext& context) {
    for (const String& entry : directory_entries(context.paths_generated)) {
      if (!remove(entry)) {
        make_ref<Error>(ERROR, format("Failed to remove \"%s\"", entry.c_str()));
      }
    }

    return ProgramError::OK;
  }

  Ref<ProgramError> ClearCache::run(ApplicationContext& context) {
    for (const String& entry : directory_entries(context.paths_cache)) {
      if (!remove(entry)) {
        make_ref<Error>(ERROR, format("Failed to remove \"%s\"", entry.c_str()));
      }
    }

    return ProgramError::OK;
  }

}  // namespace GodotObjectCompiler