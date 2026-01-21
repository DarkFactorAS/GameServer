# BotWebServer Version Management

This project now includes automated version management for the BotWebServer application. The version is stored in `BotWebServer/Program.cs` and can be updated through multiple methods.

## Automated GitHub Deployment

The GitHub Actions deployment workflow (`.github/workflows/botserver-deploy.yml`) now automatically updates the version based on the deployment trigger:

### Version Update Behavior

- **GitHub Releases**: Uses the release tag as version (removes 'v' prefix if present)
- **Merged Pull Requests**: Auto-increments the patch version (e.g., 1.0.1 → 1.0.2)
- **Manual Workflow Dispatch**: Uses date-based version (e.g., 1.0.240121 for Jan 21, 2024)

### Workflow Steps

1. **Extract Version**: Determines version from GitHub context
2. **Update Program.cs**: Modifies the version string in the source code
3. **Commit Changes**: Commits version update back to repository with `[skip ci]` tag
4. **Deploy**: Proceeds with normal staging and production deployment

## Local Development Scripts

### PowerShell Script (`update-version.ps1`)

```powershell
# Set specific version
.\update-version.ps1 -Version "1.2.3"

# Auto-increment patch version
.\update-version.ps1 -AutoIncrement

# Show help
.\update-version.ps1 -Help
```

### Build Scripts

#### Windows (`build-botwebserver.cmd`)
```batch
# Build with current version
build-botwebserver.cmd

# Build and auto-increment version
build-botwebserver.cmd /auto-increment

# Build with specific version
build-botwebserver.cmd /version:1.2.3

# Skip version update
build-botwebserver.cmd /skip-version
```

#### Linux/macOS (`build-botwebserver.sh`)
```bash
# Build with current version
./build-botwebserver.sh

# Build and auto-increment version
./build-botwebserver.sh --auto-increment

# Build with specific version
./build-botwebserver.sh --version 1.2.3

# Skip version update
./build-botwebserver.sh --skip-version
```

## Version Format

All scripts use semantic versioning format: `MAJOR.MINOR.PATCH`

- **MAJOR**: Breaking changes
- **MINOR**: New features (backward compatible)
- **PATCH**: Bug fixes (backward compatible)

## Integration with Existing Workflow

The version management integrates seamlessly with your existing infrastructure:

- ✅ Works with existing Docker builds
- ✅ Compatible with staging/production deployment pipeline
- ✅ Maintains compatibility with reusable Infrastructure workflows
- ✅ Includes proper dependency ordering (version update → staging → production)

## Troubleshooting

### PowerShell Execution Policy

If you encounter execution policy errors, run scripts with:
```powershell
powershell.exe -ExecutionPolicy Bypass -File .\update-version.ps1 [parameters]
```

### Git Permissions

Ensure the GitHub Actions have proper permissions to push commits back to the repository. The workflow uses `secrets.GITHUB_TOKEN` which should have the necessary permissions.

### Version Conflicts

If multiple developers are working on version updates, merge conflicts might occur. Use the auto-increment feature for feature branches and manual versioning for releases.

## Files Modified

- `.github/workflows/botserver-deploy.yml` - Added version update job
- `update-version.ps1` - PowerShell version management script
- `build-botwebserver.cmd` - Windows build script with version management
- `build-botwebserver.sh` - Linux/macOS build script with version management
- `VERSION_MANAGEMENT.md` - This documentation file

## Next Steps

1. Test the workflow with a sample PR or release
2. Verify that version updates appear correctly in deployment logs
3. Consider adding version display to a health check endpoint
4. Update any CI/CD documentation to include version management procedures