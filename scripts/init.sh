#!/bin/bash

CLANG=$(which clang++)
CMAKE=$(which cmake)
CLANG_FORMAT=$(which clang-format)
CLANG_TIDY=$(which clang-tidy)
BEAR=$(which bear)

if [[ -z "$CLANG" || -z "$CMAKE" || -z "$CLANG_FORMAT" || -z "$CLANG_TIDY" || -z "$BEAR" ]]; then
    echo "Please install clang, cmake, clang-format, clang-tidy and bear"
    sudo dnf install clang clang-tools-extra cmake ninja-build lld bear -y
 -y
fi

pre_commit_path=$(git rev-parse --show-toplevel)/.git/hooks/pre-commit
rm $pre_commit_path
cp $(find $PWD/.. -name 'pre-commit') $(git rev-parse --show-toplevel)/.git/hooks/ 2> /dev/null \
    && chmod +x $(git rev-parse --show-toplevel)/.git/hooks/pre-commit

commit_msg_path=$(git rev-parse --show-toplevel)/.git/hooks/commit-msg
rm $commit_msg_path
cp $(find $PWD/.. -name 'commit-msg') $(git rev-parse --show-toplevel)/.git/hooks/ 2> /dev/null \
    && chmod +x $(git rev-parse --show-toplevel)/.git/hooks/commit-msg

echo "Hooks installed successfully"
