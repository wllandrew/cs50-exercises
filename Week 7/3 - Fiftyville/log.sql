-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Check database schema
.schema

-- Look for description in crime scene reports for the day the theft took place.
SELECT description FROM crime_scene_reports WHERE day = 28 AND month = 7 AND year = 2024;

-- Look for information on the interview taken on this day.
SELECT transcription FROM interviews WHERE day = 28 AND monht = 7 AND year = 2024;

-- Three useful transcription describe the crime scene. Lets look at each one:
-- 1: Bakery Security Logs -> The culprit is one of these people:
SELECT DISTINCT name FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE day = 28 AND month = 7 AND year = 2024 AND hour = 10 AND minute >= 5 AND minute <= 25;

-- 2.1: Look at the atm machine records. But first find out when eugene arrived at the bakery
SELECT hour, minute FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE name = 'Eugene' AND activity = 'entrance' AND hour <= 10;
-- After we find out eugene arrived at the bakery at 8:53, let look at the atm records:
-- 2.2: Search for the thiefs name in transactions and compare it to the list of names we already have.
SELECT name FROM atm_transactions
JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
JOIN people ON people.id = bank_accounts.person_id
WHERE day = 28 AND month = 7 AND year = 2024 AND transaction_type = 'withdraw'
AND name IN
(
SELECT DISTINCT name FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE day = 28 AND month = 7 AND year = 2024 AND hour = 10 AND minute >= 5 AND minute <= 25
);

-- 3: Lets look at phone calls for that day:
SELECT name FROM phone_calls
JOIN people ON phone_calls.caller = people.phone_number
WHERE day = 28 AND month = 7 AND year = 2024 AND duration < 60
AND name IN
(
SELECT name FROM atm_transactions
JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
JOIN people ON people.id = bank_accounts.person_id
WHERE day = 28 AND month = 7 AND year = 2024 AND transaction_type = 'withdraw'
AND name IN
(
SELECT DISTINCT name FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE day = 28 AND month = 7 AND year = 2024 AND hour = 10 AND minute >= 5 AND minute <= 25
)
);
-- The output gives us two names: Bruce and Diana

-- 4: Lets analyse the flights for 29/07/2024, where the robber took the earliest flight
-- 4.1: Find out the time of the earlist flight:
SELECT MIN(hour) FROM flights
JOIN airports ON airports.id = flights.origin_airport_id
WHERE city = 'Fiftyville' AND day = 29 AND month = 7 AND year = 2024;
-- First you find the minimum hour, then minimum minute.
SELECT MIN(minute) FROM flights
JOIN airports ON airports.id = flights.origin_airport_id
WHERE city = 'Fiftyville' AND day = 29 AND month = 7 AND year = 2024
AND hour = 8;

-- 4.2 After finding out the earliest flight is at 8:20 am, lets look at the flight itself.
SELECT name FROM flights
JOIN passengers ON passengers.flight_id = flights.id
JOIN people ON people.passport_number = passengers.passport_number
JOIN airports ON airports.id = flights.origin_airport_id
WHERE city = 'Fiftyville'
AND day = 29 AND month = 7 AND year = 2024 AND hour = 8 AND hour = 8 AND minute = 20
AND name in
(
SELECT name FROM phone_calls
JOIN people ON phone_calls.caller = people.phone_number
WHERE day = 28 AND month = 7 AND year = 2024 AND duration < 60
AND name IN
(
SELECT name FROM atm_transactions
JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
JOIN people ON people.id = bank_accounts.person_id
WHERE day = 28 AND month = 7 AND year = 2024 AND transaction_type = 'withdraw'
AND name IN
(
SELECT DISTINCT name FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE day = 28 AND month = 7 AND year = 2024 AND hour = 10 AND minute >= 5 AND minute <= 25
)
)
);

-- The only name in the list is bruce, so the robber is Bruce.
-- Lets find out the city he went to.
SELECT city FROM airports
JOIN flights ON flights.destination_airport_id = airports.id
WHERE origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville')
AND day = 29 AND month = 7 AND year = 2024 AND hour = 8 AND minute = 20;

-- We found out he went to NYC

-- The only thing left to find is to find the accomplice by looking at the phone call
SELECT name FROM phone_calls
JOIN people ON people.phone_number = phone_calls.receiver
WHERE caller = (SELECT phone_number FROM people WHERE name = 'Bruce')
AND day = 28 AND month = 7 AND YEAR = 2024 AND duration < 60;

-- The accomplice is robin.
