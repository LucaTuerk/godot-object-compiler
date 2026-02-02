
#include "clear.h"

#include "library/core/helpers.h"

namespace GodotObjectCompiler {

  Ref<ProgramError> Clear::run(ApplicationContext& context) {
    ClearCache clear_cache;
    Ref<ProgramError> clear_cache_error = clear_cache.run(context);
    if (clear_cache_error != ProgramError::OK) {
      return clear_cache_error;
    }

    ClearGenerated clear_generated;
    Ref<ProgramError> clear_generated_error = clear_generated.run(context);
    if (clear_generated_error != ProgramError::OK) {
      return clear_generated_error;
    }

    return ProgramError::OK;
  }

  Ref<ProgramError> ClearGenerated::run(ApplicationContext& context) {
    for (const String& file : directory_files_recursive(context.paths_generated)) {
      if (!remove_file(file)) {
        make_ref<Error>(ERROR, format("Failed to remove file \"%s\"", file.c_str()));
      }
    }
    return ProgramError::OK;
  }

  Ref<ProgramError> ClearCache::run(ApplicationContext& context) {
    for (const String& file : directory_files_recursive(context.paths_cache)) {
      if (!remove_file(file)) {
        make_ref<Error>(ERROR, format("Failed to remove file \"%s\"", file.c_str()));
      }
    }

    return ProgramError::OK;
  }

}  // namespace GodotObjectCompiler