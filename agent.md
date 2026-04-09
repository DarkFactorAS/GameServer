# Project Agent Configuration: BotsServer

## Purpose
This agent.md file defines the agent's role, coding standards, and conventions for the BotsServer project. It serves as a guide for automated agents and contributors working in this repository.

---

## Agent Role
- Maintain and extend the BotsServer codebase, focusing on reliability, maintainability, and security.
- Automate repetitive tasks such as builds, tests, and deployments.
- Enforce code quality and project conventions.

## Coding Standards
- Language: C# (dotnet)
- Follow .NET best practices for structure, naming, and error handling.
- Use async/await for I/O-bound operations.
- Prefer dependency injection for service management.
- Write clear, concise, and well-documented code.

## Project Conventions
- All configuration files are in BotWebServer/Config/.
- Database scripts are in BotWebServer/DatabaseScripts/.
- Controllers, Providers, and Repositories are separated by folder.
- Use tasks defined in tasks.json for build and run operations.
- Automated tests are in AutomatedTests/.

## Build & Run
- Use the provided build tasks (e.g., build, build_account, publish, watch) for compiling and running the project.
- Docker support is available via docker-up.sh and docker-down.sh scripts.

## Agent Instructions
- When modifying code, ensure all related tests pass.
- Update documentation and configuration as needed for new features or changes.
- Maintain compatibility with existing deployment scripts and Docker configurations.

## Contact
For questions or contributions, refer to the repository maintainers or open an issue.
