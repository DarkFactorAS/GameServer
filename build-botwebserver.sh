#!/bin/bash

# Build script for BotWebServer with version management
# Usage: ./build-botwebserver.sh [--version VERSION] [--auto-increment] [--skip-version]

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROGRAM_FILE="$SCRIPT_DIR/BotWebServer/Program.cs"

# Parse command line arguments
VERSION=""
AUTO_INCREMENT=false
SKIP_VERSION=false

while [[ $# -gt 0 ]]; do
    case $1 in
        --version)
            VERSION="$2"
            shift 2
            ;;
        --auto-increment)
            AUTO_INCREMENT=true
            shift
            ;;
        --skip-version)
            SKIP_VERSION=true
            shift
            ;;
        --help)
            echo "Build script for BotWebServer with version management"
            echo ""
            echo "Usage: $0 [OPTIONS]"
            echo ""
            echo "Options:"
            echo "  --version VERSION     Set specific version (e.g., 1.2.3)"
            echo "  --auto-increment      Increment patch version automatically"
            echo "  --skip-version        Skip version update"
            echo "  --help               Show this help message"
            echo ""
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Function to update version in Program.cs
update_version() {
    local new_version="$1"
    
    # Extract current version
    current_version=$(grep -o 'Version = "[^"]*"' "$PROGRAM_FILE" | cut -d'"' -f2)
    echo "Current version: $current_version"
    
    # Update version in file
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        sed -i '' "s/public static string Version = \"[^\"]*\";/public static string Version = \"$new_version\";/" "$PROGRAM_FILE"
    else
        # Linux
        sed -i "s/public static string Version = \"[^\"]*\";/public static string Version = \"$new_version\";/" "$PROGRAM_FILE"
    fi
    
    echo "✅ Version updated to: $new_version"
}

# Handle version update
if [[ "$SKIP_VERSION" == false ]]; then
    if [[ -n "$VERSION" ]]; then
        update_version "$VERSION"
    elif [[ "$AUTO_INCREMENT" == true ]]; then
        # Extract current version and increment patch
        current_version=$(grep -o 'Version = "[^"]*"' "$PROGRAM_FILE" | cut -d'"' -f2)
        IFS='.' read -ra ADDR <<< "$current_version"
        major=${ADDR[0]}
        minor=${ADDR[1]}
        patch=${ADDR[2]}
        patch=$((patch + 1))
        new_version="$major.$minor.$patch"
        update_version "$new_version"
    else
        echo "ℹ️  Keeping current version (use --auto-increment or --version to update)"
    fi
fi

echo "🔨 Building BotWebServer..."

# Navigate to project directory
cd "$SCRIPT_DIR/BotWebServer"

# Restore dependencies
echo "📦 Restoring NuGet packages..."
dotnet restore

# Build the project
echo "🏗️  Building project..."
dotnet build --configuration Release

# Check if build was successful
if [[ $? -eq 0 ]]; then
    echo "✅ Build completed successfully!"
    
    # Display current version
    current_version=$(grep -o 'Version = "[^"]*"' "$PROGRAM_FILE" | cut -d'"' -f2)
    echo "📋 Current version: $current_version"
else
    echo "❌ Build failed!"
    exit 1
fi