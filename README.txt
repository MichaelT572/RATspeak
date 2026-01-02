__________    ________________                           __
\______   \  /  _  \__    ___/___________   ____ _____  |  | __
 |       _/ /  /_\  \|    | /  ___/\____ \_/ __ \\__  \ |  |/ /
 |    |   \/    |    \    | \___ \ |  |_> >  ___/ / __ \|    <
 |____|_  /\____|__  /____|/____  >|   __/ \___  >____  /__|_ \
        \/         \/           \/ |__|        \/     \/     \/
Version 2.0.0 - Created by Michael Torres

--------------------------------------------------------------------------------

This is an esoteric interpreted programming language based off of rats. Refer below
for syntax/functions:

Run with:
RATspeak FILENAME

Function declaration:
maze FUNCNAME <
	...
>

Main function:
squeak <
	...
>

Objects: 🐀🧀💊🕳️🏭🧪🧱🗑🔼🧵📡☣ (Print with RATspeak -e)

Built in functions:
🐀<x,y>; - Places rat at (x, y)
🐀<x,y,w>; - Places rat at (x, y) with weight w

🧀<x,y>; - Places cheese at (x, y)
🧀<x,y,c>; - Places cheese at (x, y) with count c

💊<x,y>; - Places pill at (x, y)
💊<x,y,c>; - Places pill at (x, y) with count c

🕳️<x,y>; - Places rat generator at (x, y)
🕳️<x,y,0/1>; - Places rat generator at (x, y), 1 to pull user input (user input is converted to a list of chars, placed in the +y direction)

🏭<x,y>; - Places cheese generator at (x, y)
🏭<x,y,0/1>; - Places cheese generator at (x, y), 1 to pull user input

🧪<x,y>; - Places pill generator at (x, y)
🧪<x,y,0/1>; - Places pill generator at (x, y), 1 to pull user input

🧱<x,y>; - Places wall at (x, y)
🧱<x1,y1,x2,y2>; - Places wall from (x1, y1) to (x2, y2) (must be in a line)

🗑<x,y>; - Places trash at (x, y)

🔼<x1,y1,x2,y2>; - Places catapult at (x1, y1), shooting to (x2, y2)

🧵<x,y>; - Places string at (x, y)
🧵<x1,y1,x2,y2>; - Places string from (x1, y1) to (x2, y2) (must be in a line)

📡<x,y, 0/1>; - Places scanner at (x, y), 1 to print integers

☣<x,y>; - Places poison at (x, y)

Logic:
🐀 will pathfind to nearest 🧀 (avoiding 🧱), searching at most 400 tiles for one (if none found, 🐀 does not move)
🧀 increments 🐀 weight by its count
💊 decrements 🐀 weight by its count
When 🐀 hits a 🧵 with non-negative weight, it activates all connected generators/string/catapults
🕳️, 🏭, and 🧪 spawn their respective item with activated by a string (if pulling user input, waits for input and outputs a series of objects corresponding to each ascii value in the +y direction)
🔼 moves all 🐀🧀💊 on it when activated by string
Any object thrown into 🗑 is deleted
📡 outputs 🐀 weight
If 🐀 hits ☣, the program ends

Notes on string connections:
When placing any generator, it will connect to any string at its location
When placing any SINGLE string (🧵<x,y>;), it connects all string at its location to itself)
String placed in a line connect to each other implicitely, but only the endpoints auto-connect to other string (to allow for passing lines of string past one another)

