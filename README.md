# Cafeteria Recommendation Engine

## Project Overview

This project simulates a cafeteria recommendation and feedback system designed to bridge the gap between employees, chefs, and administrators. It ensures that employees can easily decide what to order, the chef can plan meals efficiently without wastage, and the admin can manage users and menu items.

The system integrates database management, socket programming, multithreading, and real-time notifications to create a robust recommendation engine for a corporate cafeteria.

**Goal:**

- Help employees discover popular and preferred food items and combinations.
- Provide chefs with insights into user preferences, upcoming orders, and food performance.
- Allow admins to manage menus, users, and system data.
- Use feedback (likes, dislikes, comments, sentiments) to continuously improve cafeteria offerings.

## Features

#### Authentication & Authorization

- Three roles: Admin, Chef, User
- Secure login via unique ID & password.
- Role-based features and access controls.

1. **Admin Functionality**

   - Manage users (add, update, delete, view).
   - Manage menu items (add, update, delete, view).
   - Manage discarded menu items.

2. **Chef Functionality**
   - Receive notifications for employee orders and feedback.
   - Roll out daily menus (breakfast, lunch, dinner, or all-day).
   - View employees’ responses to menus.
   - View and manage discarded items (low rating/feedback).
   - Search for items and analyze likes, dislikes, comments, and sentiments.
   - Generate monthly report
3. **User Functionality**
   - View available foods and notifications.
   - Place orders for the next day’s meals (breakfast/lunch/dinner).
   - Suggest non-listed items (if >10 people suggest → chef notified).
   - Give feedback on items (like, dislike, comments).
   - Create/update food preference profile.

#### Food & Feedback System

- Menu items include: id, name, description, price, category, availability.
- Rating system based on likes/dislikes (via triggers).
- Sentiment analysis from user comments (positive/negative).
- Discarded items tracked for chef’s review.

#### Reports

- Monthly reports on orders, likes, dislikes, and most popular items.
- Automatic cleanup of notifications older than 1 week.

## Technical Highlights

- DBMS: MySQL (normalized up to 3NF)
- Triggers (auto rating updates, notification cleanup)
- Stored Procedures for repetitive operations
- Networking: Socket programming for client-server communication
- Concurrency: Multithreading for handling database access and client requests
- Queue: For notification management
- Unit Testing: Implemented using GTest and GMock

## Running the Project

#### Prerequisites

- MySQL server
- C++11 or higher (for threading and sockets)
- MySQL Connector/C++ library (mysqlcppconn)

#### Compile and Run

Run the script present in `scripts` directory from root folder:

```bash
./scripts/build.sh
```

## Future Enhancements

- Web or Mobile front-end for employees and chefs.
- Advanced sentiment analysis with ML/NLP.
- AI-based food preparation forecasting.
- Real-time analytics dashboards for admins/chefs.
- Unit tests for the project
- Configure CMake for building the project

## Demo

#### Video link:

![Demo.mp4](demo/Cafeteria_Recommendation_System.mp4)
