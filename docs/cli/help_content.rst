========================[ Godot Object Compiler v0.2.0 "Binary Bresenham" ]=========================

Usage: goc [PROGRAM PATH...] [ARGUMENTS...]

Display the required arguments for the desired program by executing the
help program with the corresponding program path, f.e:

goc help generate

Here are some common arguments you might need:

===========================================[ Arguments ]============================================

--extension_api=<Path>                                                                               
 -E=<Path>                     The path to the extension api json file to be used with your extension.
                                                                                                     

--generated_path=<Path>        Default: .goc/generated                                               
 -G=<Path>                     The directory that will be used by godot-object-compiler for generated
                               files.                                                                

--goc_path=<Path>              Default: .goc                                                         
 -P=<Path>                     The directory that will be used by godot-object-compiler for caching. 
                                                                                                     

--godot_cpp=<PathList>         A comma separated list of paths.                                      
 -GPP=<PathList>               The used godot-cpp submodules include paths.                          
                                                                                                     

--include_paths=<PathList>     Default: [], A comma separated list of paths.                         
 -I=<PathList>                 The generators include paths.                                         
                                                                                                     

--log_detail=<Flag>            Default: Full, Possible Values: Condensed, Full                       
 -D=<Flag>                     The log detail to use.                                                
                                                                                                     

--log_level=<Flag>             Default: Info, Possible Values: Error, Info, Verbose, Warning         
 -L=<Flag>                     The log level to use.                                                 
                                                                                                     

--root_path=<Path>                                                                                   
 -R=<Path>                     The projects root path.                                               
                                                                                                     

--source_parser=<Flag>         Possible Values: ClangParser, TreeSitterParser                        
 -SP=<Flag>                    The name of the source parser to be used by godot-object-compiler.    
                                                                                                     

--sources=<PathList>           Default: [], A comma separated list of paths.                         
 -S=<PathList>                 The generators target source file paths.                              
                                                                                                     

--type_db_path=<Path>          Default: .goc/cache                                                   
 -T=<Path>                     The directory that will be used by godot-object-compiler for TypeDB caching.
                                                                                                     


============================================[ PROGRAMS ]============================================

  build_num                    Show the current build hash.                                          
                                                                                                     

  clear                        Clear the type database and all generated files.                      
                                                                                                     

    generated                  Clear all generated files.                                            
                                                                                                     

    type_db                    Clear the type database.                                              
                                                                                                     

  generate                     Generate the type database and bindings.                              
                                                                                                     

    assumptions                DEV_PROGRAM - No help available                                       
                                                                                                     

    bindings                   Generate bindings for the provided source files.                      
                                                                                                     

    resources                  DEV_PROGRAM - No help available                                       
                                                                                                     

    type_db                    Generate the type database.                                           
                                                                                                     

  help                         Show this help section.                                               
                                                                                                     

init                                                                                                 
                                                                                                     

    tools                      Initialize included tool script such as build tools.                  
                                                                                                     

print                                                                                                
                                                                                                     

    parsed                     Show the parsers syntax tree output for the provided file.            
                                                                                                     

    transformed                Show the parsers transformed syntax tree output for the provided file.
                                                                                                     

    type                       Print the type tree for a list of types identified by their qualified 
                               name.                                                                 

  version                      Show the version number.                                              
                                                                                                     

=========================================[ SOURCE PARSERS ]=========================================

ClangParser                    This parser uses libclang to index your source files.                 
                               It supports macro extension and include substitution ensuring         
                               the codes correctness along the way, but will fail if your build      
                               system is not setup correctly by, for example, missing include paths. 
                               It is the only available parser option for module development.        

TreeSitterParser               This parser uses TreeSitter to index your source files.               
                               It does not support macros extension and include substitution         
                               and parses your code on a "best guess" basis, but is slightly         
                               faster and more forgiving compared to the ClangParser.                
