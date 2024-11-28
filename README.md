# RPG Game

A text-based role-playing game featuring character creation, branching storylines, and dynamic gameplay based on character stats.

## Features

- **Character Classes**: Choose your path as a Warrior, Scholar, Diplomat, or Rogue
- **Stat-Based Choices**: Your character's attributes influence story decisions
- **Dynamic Inventory**: Collect and manage items throughout your journey
- **Rich Storyline**: Experience a branching narrative across three chapters
- **Save System**: Save your progress and continue your adventure later

## Game Flow

```mermaid
graph TD
    Start([Start Game]) --> Init[Initialize Game]
    Init --> CharCreate[Character Creation]
    
    subgraph Character Creation
        CharCreate --> Name[Enter Name]
        Name --> Class[Choose Class]
        Class --> Stats[Allocate Stats]
        Stats --> Traits[Select Traits]
    end
    
    Traits --> MainLoop[Main Game Loop]
    
    subgraph Main Game Loop
        MainLoop --> Display[Display Current Scene]
        Display --> ShowChoices[Show Available Choices]
        ShowChoices --> Input[Get Player Input]
        Input --> |Save|SaveGame[Save Game]
        SaveGame --> Display
        Input --> |Choice|CheckReq{Check Requirements}
        CheckReq --> |Met|UpdateState[Update Game State]
        CheckReq --> |Not Met|Display
        UpdateState --> |Continue|Display
        UpdateState --> |Game Over|End
    end
    
    subgraph Story Progress
        UpdateState --> |Chapter Complete|NextChapter[Load Next Chapter]
        NextChapter --> Display
    end
    
    End([End Game])
```

## Technical Architecture

```mermaid
graph TB
    subgraph Game Core
        GameState[Game State Manager<br>game.h]
        CharSystem[Character System<br>character.h]
        StoryEngine[Story Engine<br>story.h]
    end

    subgraph UI Layer
        Display[Display Manager]
        Input[Input Handler]
        NCurses[NCurses Interface]
    end

    subgraph Data Management
        SaveLoad[Save/Load System]
        StoryData[Story Data]
        CharData[Character Data]
    end

    GameState --> |Manages|CharSystem
    GameState --> |Controls|StoryEngine
    StoryEngine --> |Updates|GameState
    CharSystem --> |Affects|StoryEngine

    GameState --> |Updates|Display
    Input --> |Modifies|GameState
    Display --> |Uses|NCurses
    Input --> |Uses|NCurses

    GameState --> |Saves|SaveLoad
    SaveLoad --> |Loads|GameState
    StoryEngine --> |Reads|StoryData
    CharSystem --> |Manages|CharData
```

## Story Structure

```mermaid
graph TD
    Start[Chapter Start] --> Scene1[Opening Scene]
    
    Scene1 --> |STR 5|Choice1A[Force Entry]
    Scene1 --> |INT 5|Choice1B[Find Alternative]
    Scene1 --> |CHA 5|Choice1C[Negotiate]
    
    subgraph Strength Path
        Choice1A --> |Success|Combat[Combat Scene]
        Combat --> |Victory|Reward1[Gain Weapon]
        Combat --> |Defeat|Penalty1[Lose Health]
    end
    
    subgraph Intelligence Path
        Choice1B --> |Success|Puzzle[Puzzle Scene]
        Puzzle --> |Solved|Reward2[Gain Item]
        Puzzle --> |Failed|Penalty2[Story Detour]
    end
    
    subgraph Charisma Path
        Choice1C --> |Success|Dialog[Dialog Scene]
        Dialog --> |Agreement|Reward3[Gain Ally]
        Dialog --> |Disagreement|Penalty3[Reputation Loss]
    end
    
    Reward1 --> NextScene[Next Scene]
    Penalty1 --> NextScene
    Reward2 --> NextScene
    Penalty2 --> NextScene
    Reward3 --> NextScene
    Penalty3 --> NextScene
```

## Getting Started

### Prerequisites

- GCC compiler
- Make build system
- ncurses library

On Ubuntu/Debian, install the required dependencies:
```bash
sudo apt install gcc make libncurses5-dev
```

### Building the Game

1. Clone the repository:
```bash
git clone [repository-url]
cd rpg_game
```

2. Build the game:
```bash
make clean
make
```

3. Run the game:
```bash
./bin/rpg_game
```

## Gameplay Guide

1. **Character Creation**
   - Choose your character class
   - Allocate stats (Strength, Intelligence, Charisma)
   - Select character traits

2. **Game Controls**
   - Use number keys to select choices
   - Press 'S' to save game
   - Press 'Q' to quit

3. **Stats System**
   - Strength: Affects combat and physical challenges
   - Intelligence: Influences puzzle-solving and knowledge checks
   - Charisma: Determines success in social interactions

## Project Structure

- `src/`: Source code files
  - `character.c`: Character creation and management
  - `game.c`: Core game mechanics
  - `story.c`: Story content and branching logic
- `include/`: Header files
  - `character.h`: Character system definitions
  - `game.h`: Game state and core functions
  - `story.h`: Story system structures
- `bin/`: Compiled executable
- `doc/`: Documentation (generated with Doxygen)

## Documentation

Detailed documentation is available in the `doc/` directory. To generate the documentation:

```bash
doxygen
```

Then open `doc/html/index.html` in your web browser.

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- ncurses library for terminal UI
- Inspiration from classic text-based RPGs