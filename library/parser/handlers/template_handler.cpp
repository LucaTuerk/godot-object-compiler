//
// #include "template_handler.h"
//
// #include "library/tree/syntax/function.h"
//
// namespace GodotObjectCompiler {
//
//   bool TemplateHandler::handles_node(const Ref<TreeSitterNode>& current_src) {
//     return current_src->type_in({"template_declaration", "template_parameter_list", "type_parameter_declaration"});
//   }
//
//   ParserStep TemplateHandler::handle(const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target) {
//     print_ln(current_src->pretty_print());
//     if (!current_src->has_next_sibling()) {
//       return ParserStep::StepOver();
//     }
//
//     if (current_src->type == "template_parameter_list") {
//       current_target = current_target->create_child<TemplateParameters>();
//     }
//     return ParserStep::StepInto();
//   }
//
// }  // namespace GodotObjectCompiler