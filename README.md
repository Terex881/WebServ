# 🌐 WebServ

## 📋 Project Overview

WebServ is an advanced, production-grade HTTP server implementation designed to showcase sophisticated network programming techniques and web server architecture.

## 🏗️ Technical Architecture

### Core Design Principles
- **🚀 High Performance**: Non-blocking I/O architecture
- **🔒 Robust Handling**: Comprehensive error management
- **🔀 Flexible Routing**: Dynamic configuration capabilities

### System Specifications
```
Language       : C++ 98
Concurrency    : Non-blocking socket management
Compliance     : HTTP 1.1 Standard
```

## 👥 Team Expertise

| Team Member | Primary Responsibilities |
|------------|-------------------------|
| @aibn-che  | • Configuration Parsing<br>• CGI Management<br>• Response Generation |
| @ybellakr  | • Server Architecture<br>• Multi-Request Handling<br>• Advanced Routing |
| @sdemnati  | • Request Parsing<br>• File Upload Mechanisms<br>• Method-Specific Processing |

### @aibn-che | Configuration & Core Infrastructure Specialist
**Technical Contributions:**
- Implemented robust configuration file parsing mechanism
- Developed comprehensive CGI (Common Gateway Interface) management system
- Engineered advanced HTTP response generation
- Key Responsibilities:
  - Parsing complex server configuration files
  - Managing dynamic content generation
  - Implementing chunk-based data transmission
  - Ensuring configuration integrity and flexibility

### @ybellakr | Server Architecture & Performance Engineer
**Technical Contributions:**
- Designed non-blocking server architecture using keque
- Developed multi-request handling strategy
- Implemented advanced routing and redirection mechanisms
- Key Responsibilities:
  - Creating concurrent request processing model
  - Implementing DELETE method functionality
  - Developing directory listing capabilities
  - Optimizing server resource utilization

### @sdemnati | Request Processing & Data Management Expert
**Technical Contributions:**
- Implemented comprehensive request parsing for multiple HTTP methods
- Developed sophisticated file upload mechanisms
- Created flexible data handling strategies
- Key Responsibilities:
  - Parsing complex POST, GET, and DELETE requests
  - Managing diverse file upload scenarios
  - Handling form data, query strings, and no-data uploads
  - Ensuring robust request processing.

## 🛠️ Technical Capabilities

### Configuration Features
- ✅ Multi-host and multi-port support
- ✅ Dynamic route definition
- ✅ Configurable error pages
- ✅ File upload management
- ✅ CGI script execution

### Deployment

```bash
# Compilation
make

# Server Startup
./webserv [configuration_file]
```

## 🔍 Key Constraints

- **Keque-Driven** Concurrency Model
- **Zero Downtime** Design
- Strictly **Non-Blocking** Operation
- Efficient **I/O Management**
- No Arbitrary `fork()` Usage
- Resilient Error Handling

## 🧪 Validation Strategies

1. Browser Compatibility Testing
2. NGINX Behavior Comparative Analysis
3. Comprehensive Stress Testing

## 📜 Licensing

This project is part of the 42 curriculum and is provided as-is.
