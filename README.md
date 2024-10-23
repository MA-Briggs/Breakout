# Breakout

W Kavanagh. June Summer 2024

## controls

A/D to move the paddle right and left.
P to pause.

## Powerups

big/small paddle (blue)
fast/slow ball (fancy yellow)
fire ball (green)

# Tasklist

## Suggested fixes

* Fix the compiler issues in the code

## Suggested tasks

* Implement mouse input for pad
* Improved VFX (ball trail, paddle hit response, brick particle destruction)
* Better UI (progress bar rather than timer for countdown).
* GameLoop
* Better ball physics (Box2D)
* Leaderboards
* More ball types (e.g., multiball, sticky ball [where you shoot the ball from the paddle every time], tiny ball, big ball, brick-tracking ball)
* Sounds with increasing tone between bounces.
* Implement commentary with calls to an LLM such as LLama

# Time Details and Changelist
Total time spent working on project: 03:53:00

Fixed compiler error caused by incorrect include in PowerupBase.h - 23 minutes

Set up .txt file creation to store leaderboard,
Implemented user text input,
Implemented write to file - 1 hour

Implemented text display for users,
Set up validations for functions which use objects that get deleted for the leaderboard function, - 49 minutes

Implemented read from file,
sorted data based on time - 1 hour 27 minutes

Set up the display for the leaderboard,
Testing - 14 minutes
