========================[ Godot Object Compiler v0.1 "public experimental" ]========================

Usage: goc [PROGRAM PATH...] [PROGRAM ARGS...] [APPLICATION ARGS...]

The godot-object-compiler requires a project for most programs, project free
programs are marked [!p] in the program list.
The project can either be provided via a .goc_project file in the working directory OR via
the following application arguments:


--root_path=, -R=
    The projects root path, including the generator target files

--goc_path=, -P=
    The folder the godot-object-compiler can place files in

--generated_path=, -G=
    The folder the godot-object-compiler should place generated files in

--cache_path=, -C=
    The godot-object-compiler cache folder, including the type database

--include_paths=, -I=
    The generators include paths, comma seperated. Must include the godot-cpp
    include and gen/include paths.

--sources=, -S=
    The generators target source file paths, comma seperated.

--godot_cpp=, -GPP=
    The path to the godot-cpp repositories root used with your extension.

--extension_api=, -E=
    The path to the extension api json file to be used with your extension.

--source_parser=, -SP=
    The name of the source parser to be used by godot-object-compiler.


=========================================[ SOURCE PARSERS ]=========================================

ClangParser                    This parser uses libclang to index your source files.                 
                               It supports macro extension and include substitution ensuring         
                               the codes correctness along the way, but will fail if your build      
                               system is not setup correctly by, for example, missing include paths. 
                               It is the only available parser option for module development.        

TreeSitterParser (Default)     This parser uses TreeSitter to index your source files.               
                               It does not support macros extension and include substitution         
                               and parses your code on a "best guess" basis, but is slightly         
                               faster and more forgiving compared to the ClangParser.                

============================================[ PROGRAMS ]============================================

build_num [!p]                 Show the current build hash.                                          

clear                          Clear the type database and all generated files.                      

  generated                    Clear all generated files.                                            

  type_db                      Clear the type database.                                              

generate                       Generate the type database and bindings.                              

  assumptions                  DEV_PROGRAM - No help available                                       

  bindings                     Generate bindings for the provided source files.                      

  resources                    DEV_PROGRAM - No help available                                       

  type_db                      Generate the type database.                                           

help [!p]                      Show this help section.                                               

init [!p]                      Interactively initialize a .goc_project file in this directory.       

  tools [!p]                   Initialize included tool script such as build tools.                  
                               [args]                                                                
                                   <directory path> required - path of the folder to copy files to.  

print                                                                                                

  parsed [!p]                  Show the parsers syntax tree output for the provided file.            
                               [args]                                                                
                                   <file path> required - path to the file to parse                  

  transformed                  Show the parsers transformed syntax tree output for the provided file.
                               [args]                                                                
                                   <file path> required - path to the file to parse                  

  type                         Print the type tree for a list of types identified by their qualified 
                               name.                                                                 
                               [args]                                                                
                                   <qualified name...> required - The types qualified names          

version [!p]                   Show the version number.                                              
