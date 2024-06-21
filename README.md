**Task Description:** 2D Point Motion Tracking Application

**Objective:**

- Create an application that handles points on a 2D plane and allows users to interact with them.
	- The application should provide through an interface the telemetry of the points, namely the {x,y} coordinates of each point.
	- The application should also provide an interface so that the user can set new positions of the points.
	- The application should implement a smooth motion between the current position and the new, user-defined position for each point.
	- The application needs to be a standalone executable.

**Application Command-line Input:**

- On start-up, the application needs to get the initial number of points on the 2D plane. This is something that should not change throughout the execution of the application. More cli arguments are allowed (it depends on the solution is they are needed or not), but the number of points is mandatory.

**User Input:**

- While the application runs, the user needs to be able to set new positions for the points.

**Telemetry:**

- The telemetry should include both X and Y coordinates for each point at each given time.

**Communication Channel:**

- Implement a communication channel between the user and the application for the telemetry and the setting of the new point positions.

**Point Motion:**

- Implement a mechanism for updating the position of each point, based on its current one the on the new one defined by the user, in a smooth way (e.g. constant velocity or acceleration profile).
- The points should not move as long as the user does not set new positions for them.
- Collisions during motion can be ignored.

**Code:**

- The `Point` class represents one point.
- The `PointManager` class handles all the points.

**Notes:**
- Use only C++ for the application and/or Python for scripting or other functionality (if necessary).
- Use CMake for building the application.
- Assume Ubuntu 22.04 as the base system.
- The infrastracture of a communication channel based on sockets is already provided. You can either use that or implement your own communication mechanism.
- The API of the interfaces is up to you to design.

**Deliverables:**
- Source code
- Necessary files for building the application
- Docker configuration file for building and running the application
- Provide your solution as a git repository, please do not use public hosting services like github or gitlab. Instead, send back an archive with the git. Ensure that commit history reflects the progression of work on a real task.
