//
// Created by luca on 03.02.26.
//

#include "init_local_resources.h"

#include "program_functions.h"

namespace GodotObjectCompiler {

  Ref<ProgramError> InitLocalResources::run(ApplicationContext& p_context) {
    if (!ProgramFunctions::copy_resources_to_folder(
            {
                "res://variant_types",
                "res://macros",
            },
            p_context.paths_goc)) {
      return make_ref<ProgramError>(ERROR, "Failed to copy some local resources.");
    }
    return ProgramError::OK;
  }

}  // namespace GodotObjectCompiler