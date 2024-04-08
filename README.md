# Connect-Four
This was a class project that's designed to play Connect-Four, whether it's against an AI or Player vs. Player. 

# Disclaimer
Since this is a class project, some functions were given to me and therefore are not of my own creation. Therefore, I cannot make this project public. However, my AI algorithm is solely my own creation. As such, I will provide public access to that function, but not the rest of the codes that allow the function to run. However, I will create demo videos to show how my AI algorithm works. 

# DESCRIPTION - STRATEGY
1. The algorithm's number one goal is to check if it can win immediately
2. The next goal is to block a potential immediate win
3. If there's no immediate winning potential then it'll check for a longer-term winning potential.
   This step essentially checks to see if there are 2 tiles that's connected
   or if there's one tile that can be connected to make 2 connected tiles.
4. Lastly, if there are no winning potential, it'll go from left-to-right and drop a tile at the first BLANK
