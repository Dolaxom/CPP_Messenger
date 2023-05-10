CREATE TABLE users (
  id SERIAL PRIMARY KEY,
  username VARCHAR(50) UNIQUE NOT NULL,
  password VARCHAR(50) NOT NULL,
  session_token INTEGER DEFAULT -1
);

CREATE TABLE messages (
    id SERIAL PRIMARY KEY,
    sender_name VARCHAR(50) NOT NULL,
    receiver_name VARCHAR(50) NOT NULL,
    message TEXT NOT NULL,
    sent_time TIMESTAMP NOT NULL DEFAULT NOW()
);