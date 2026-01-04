#!/bin/bash
# Install git hooks for this repository

HOOK_DIR="$(git rev-parse --show-toplevel)/.githooks"

if [ ! -d "$HOOK_DIR" ]; then
    echo "Error: .githooks directory not found"
    exit 1
fi

# Configure git to use our hooks directory
git config core.hooksPath .githooks

# Make hooks executable
chmod +x "$HOOK_DIR/pre-commit" 2>/dev/null
chmod +x "$HOOK_DIR/pre-push" 2>/dev/null

echo "Git hooks installed successfully!"
echo "  - pre-commit: format and lint checks"
echo "  - pre-push: full test suite"
