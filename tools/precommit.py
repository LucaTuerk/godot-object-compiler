from create_all_h import create_all_h_for_project
from add_licence_headers import add_licence_headers_for_project
from format import format_project


def precommit():
    print("Running precommit.")
    add_licence_headers_for_project()
    create_all_h_for_project()
    format_project()


if __name__ == "__main__":
    precommit()
