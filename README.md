# Clink-Click
This is an attempt at a single-player combat game, with a single powerful enemy that tracks you through a maze of obstacles. I am also trying to add a feature wherein if you try going beyond the edge of a map, you will enter another map... it's tricky.

# Project summary

## Purpose
Video games have always fascinated me. Not as much their graphical aspects (for which I have deep respect and admiration) as their logical aspects. How do NPC's move in relation to the player? How are the various mechanics such as inventory management, motion and combat implemented?

I attempted to recreate some of these ideas using the knowledge I had, which was fairly limited, as is reflected in the limited capacities of the game. I am proud of this work, but it can be so much more.

## Features
- Turn based movement
- Arrow key controls
- Directional strikes (using WASD)
- Inventory (for switching weapons and collected items)
- Ranged, melee and tool-based weapons
- Ability to collect and replace the obstacles
- Pathfinding enemy (breaks in some scenarios)
- Powerful enemy

## Potential features
- Multiple maps
- Expanded space to move (involving dynamic map switching, based on the arrangement of the locations in relation to each other)
- Smarter enemy (especially with improved pathfinding)
- More enemies of different varieties

## Conclusion

### Result
Other than the pathfinding, everything in the code works robustly. I am especially happy with the player's ability to mine, collect and replace the obstacles in the arena. Even the pathfinding works to a great extent, and only breaks in some scenarios. However, the game is limited, and the objective (of killing the enemy) is either boring (the enemy is too weak) or impossible (the enemy is too strong), based on the settings I apply to the parameters in my code. I need to work on expanding the objectives.

### Benefit in organisation and separation of functionalities
For such a relatively complex program, I would have had a much harder time had I not organised the ideas, functionalities and implementations carefully BEFORE any coding took place. I initially spent a good amount of time figuring out my approach and the logic of the game before writing a single line of code, and I think I want to maintain this practice for future projects.

### Flaw in my approach
My approach was less about learning from the available body of knowledge and more about recreating such ideas using my own knowledge as far as possible. This is not the best approach to learning, I have realised, as it is extremely time-consuming and ultimately a poor use of available resources to accelerate my learning.
