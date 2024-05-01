<?php
date_default_timezone_set('Asia/Kolkata');
// Database connection details
$servername = "localhost";
$username = "root";
$password = "";
$database = "sensordata";

// Create connection
$conn = new mysqli($servername, $username, $password, $database);

// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

// Check if POST data exists for each parameter
$temperature = isset($_POST['temperature']) ? $_POST['temperature'] : null;
$humidity = isset($_POST['humidity']) ? $_POST['humidity'] : null;
$mq2_analog = isset($_POST['mq2_analog']) ? $_POST['mq2_analog'] : null;
$mq2_digital = isset($_POST['mq2_digital']) ? $_POST['mq2_digital'] : null;
$mq9_analog = isset($_POST['mq9_analog']) ? $_POST['mq9_analog'] : null;
$mq9_digital = isset($_POST['mq9_digital']) ? $_POST['mq9_digital'] : null;

// Get current date/time
$currentDateTime = date("Y-m-d H:i:s");

// Insert data into database including date/time
$sql = "INSERT INTO es_data (temperature, humidity, mq2_analog, mq2_digital, mq9_analog, mq9_digital, date_time)
VALUES ('$temperature', '$humidity', '$mq2_analog', '$mq2_digital', '$mq9_analog', '$mq9_digital', '$currentDateTime')";

if ($conn->query($sql) === TRUE) {
  echo "New record inserted successfully";
} else {
  echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
?>
