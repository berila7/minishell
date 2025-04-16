#!/bin/bash

# Set directories for plugins
ZSH_CUSTOM=${HOME}/.oh-my-zsh/custom
AUTOJUMP_DIR=${HOME}/.local/autojump

# Function to clone a GitHub repository
clone_repo() {
    local repo_url=$1
    local dest_dir=$2

    if [ -d "$dest_dir" ]; then
        echo "Directory $dest_dir already exists. Skipping clone."
    else
        echo "Cloning $repo_url into $dest_dir..."
        git clone "$repo_url" "$dest_dir"
    fi
}

# Install zsh-autosuggestions
install_zsh_autosuggestions() {
    local autosuggestions_dir="$ZSH_CUSTOM/plugins/zsh-autosuggestions"
    clone_repo "https://github.com/zsh-users/zsh-autosuggestions" "$autosuggestions_dir"

    if ! grep -q "zsh-autosuggestions" "${HOME}/.zshrc"; then
        echo "Adding zsh-autosuggestions to plugins in .zshrc..."
        sed -i 's/plugins=(\(.*\))/plugins=(\1 zsh-autosuggestions)/' "${HOME}/.zshrc"
    fi
}

# Install autojump
install_autojump() {
    clone_repo "https://github.com/wting/autojump.git" "$AUTOJUMP_DIR"

    echo "Installing autojump..."
    cd "$AUTOJUMP_DIR" && ./install.py

    if ! grep -q "autojump.zsh" "${HOME}/.zshrc"; then
        echo "Adding autojump initialization to .zshrc..."
        echo "[[ -s ${AUTOJUMP_DIR}/share/autojump/autojump.zsh ]] && source ${AUTOJUMP_DIR}/share/autojump/autojump.zsh" >> "${HOME}/.zshrc"
    fi

    if ! grep -q "autojump" "${HOME}/.zshrc"; then
        echo "Adding autojump to plugins in .zshrc..."
        sed -i 's/plugins=(\(.*\))/plugins=(\1 autojump)/' "${HOME}/.zshrc"
    fi
}

# Apply changes to .zshrc
apply_changes() {
    echo "Sourcing .zshrc to apply changes..."
    source "${HOME}/.zshrc"
}

# Main script execution
echo "Starting installation of zsh plugins..."

install_zsh_autosuggestions
install_autojump
apply_changes

echo "Installation completed! Restart your terminal to fully apply changes."
