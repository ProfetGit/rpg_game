#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "../include/story.h"
#include "../include/game.h"

StoryNode* createStoryNode(int id, const char* description) {
    StoryNode* node = (StoryNode*)malloc(sizeof(StoryNode));
    if (!node) return NULL;

    node->id = id;
    strncpy(node->description, description, MAX_DESCRIPTION_LENGTH - 1);
    node->description[MAX_DESCRIPTION_LENGTH - 1] = '\0';
    node->numChoices = 0;
    node->consequence = NULL;

    return node;
}

void addChoice(StoryNode* node, const char* choiceText, StoryNode* nextNode, int reqStr, int reqInt, int reqCha) {
    if (node->numChoices < MAX_CHOICES) {
        strncpy(node->choices[node->numChoices], choiceText, MAX_CHOICE_TEXT - 1);
        node->choices[node->numChoices][MAX_CHOICE_TEXT - 1] = '\0';
        node->nextNodes[node->numChoices] = nextNode;
        node->requirements[node->numChoices][0] = reqStr;
        node->requirements[node->numChoices][1] = reqInt;
        node->requirements[node->numChoices][2] = reqCha;
        node->numChoices++;
    }
}

void displayChoices(const StoryNode* node, const GameState* game) {
    attron(COLOR_PAIR(COLOR_CHOICE_PAIR));
    mvprintw(6, 1, "What will you do?\n\n");
    
    int y = 8;
    for (int i = 0; i < node->numChoices; i++) {
        mvprintw(y, 1, "%d. %s", i + 1, node->choices[i]);
        const int* req = node->requirements[i];
        
        // Check if the player meets the requirements and color accordingly
        if (req[0] > 0 || req[1] > 0 || req[2] > 0) {
            attron(COLOR_PAIR(COLOR_STAT_PAIR));
            printw(" (Requires: ");
            if (req[0] > 0) {
                if (game->player->strength >= req[0]) {
                    attron(COLOR_PAIR(COLOR_HEADER_PAIR));
                } else {
                    attron(COLOR_PAIR(COLOR_ERROR_PAIR));
                }
                printw("STR %d ", req[0]);
            }
            if (req[1] > 0) {
                if (game->player->intelligence >= req[1]) {
                    attron(COLOR_PAIR(COLOR_HEADER_PAIR));
                } else {
                    attron(COLOR_PAIR(COLOR_ERROR_PAIR));
                }
                printw("INT %d ", req[1]);
            }
            if (req[2] > 0) {
                if (game->player->charisma >= req[2]) {
                    attron(COLOR_PAIR(COLOR_HEADER_PAIR));
                } else {
                    attron(COLOR_PAIR(COLOR_ERROR_PAIR));
                }
                printw("CHA %d", req[2]);
            }
            attron(COLOR_PAIR(COLOR_STAT_PAIR));
            printw(")");
        }
        y++;
    }
    attroff(COLOR_PAIR(COLOR_CHOICE_PAIR));
    refresh();
}

// Chapter 1 story content
StoryNode* createChapterOne(void) {
    // Opening scene
    StoryNode* start = createStoryNode(1, 
        "The ancient city of Eldara stands as a testament to a forgotten age, its alabaster towers piercing the crimson sky like ancient spears. "
        "You've journeyed for months across treacherous lands, following whispers of the Celestial Crown - an artifact of immense power said to grant its wielder dominion over fate itself. "
        "As you crest the final hill, the city's grandeur takes your breath away. The setting sun bathes its walls in golden light, while shadows dance between its spires. "
        "Three paths lie before you, each holding its own promises and perils..."
    );

    // Main gate approach
    StoryNode* mainGate = createStoryNode(2,
        "The main gate looms before you, its ancient stone arch carved with intricate runes that pulse with a faint blue light. "
        "Two rows of city guards stand at attention, their armor bearing the mark of the Eldaran Elite - golden serpents coiled around silver swords. "
        "Their captain, a scarred veteran with calculating eyes, watches your approach with evident suspicion. "
        "You notice some guards discretely touching their weapons, while others exchange meaningful glances."
    );

    // Secret path
    StoryNode* secretPath = createStoryNode(3,
        "Your keen eyes spot an ancient aqueduct entrance, half-hidden behind a curtain of phosphorescent vines. "
        "The stonework bears the hallmarks of the Old Kingdom, predating Eldara itself. "
        "A faint humming emanates from within, and you catch glimpses of strange symbols etched into the walls. "
        "The air grows cooler as you approach, carrying the scent of ancient magic and forgotten secrets."
    );

    // Merchant route
    StoryNode* merchantCaravan = createStoryNode(4,
        "A bustling merchant caravan prepares to enter the city. Traders from distant lands haggle over exotic wares, "
        "while guards in mismatched armor patrol the perimeter with casual indifference. "
        "You spot several merchants wearing the purple sash of the Traders' Guild - a powerful organization with influence throughout the realm. "
        "Their lead merchant, a sharp-eyed woman in elaborate robes, seems to be organizing the final preparations."
    );

    // Main gate outcomes
    StoryNode* confrontGuards = createStoryNode(5,
        "You stand your ground as the captain approaches, his hand resting on his sword hilt. "
        "'State your business in Eldara, stranger,' he demands, his voice carrying years of authority. "
        "'These are troubled times, and we've had our fill of... adventurers.' "
        "The other guards tense, ready to act on their captain's command. "
        "Your response could mean the difference between entry and imprisonment."
    );

    StoryNode* bribeGuards = createStoryNode(6,
        "You catch the eye of a younger guard who keeps glancing at your coin purse. "
        "His weathered armor and worn boots suggest someone struggling to make ends meet. "
        "When your gazes meet, he gives an almost imperceptible nod toward a quiet corner of the gatehouse. "
        "The captain seems distracted with other matters - this might be your chance."
    );

    // Secret path outcomes
    StoryNode* ancientPassage = createStoryNode(7,
        "The aqueduct tunnels twist beneath the city like a stone labyrinth. "
        "Ancient magical lights flicker to life as you pass, revealing intricate mosaics depicting a forgotten history. "
        "You hear whispers in an ancient tongue, and the air thrums with arcane energy. "
        "A fork in the tunnel presents two paths: one leading deeper into darkness, the other toward a shaft of daylight."
    );

    StoryNode* magicalEncounter = createStoryNode(8,
        "The darkness ahead suddenly illuminates with ethereal blue light. "
        "A spectral figure materializes - a scholar from the Old Kingdom, his translucent form flickering with residual magic. "
        "'Seeker,' he intones, 'you walk paths untraveled for centuries. What knowledge do you pursue in these halls?'"
    );

    // Merchant outcomes
    StoryNode* merchantDeal = createStoryNode(9,
        "The lead merchant, introducing herself as Lyra Blackwood, listens to your story with growing interest. "
        "'The Celestial Crown, you say?' she muses, twirling a golden ring. 'Now that's a name I haven't heard in some time. "
        "Perhaps we could help each other. I have contacts within the city, but such information... it comes at a price.'"
    );

    StoryNode* merchantBetrayal = createStoryNode(10,
        "As you speak with Lyra, you notice several of her guards quietly repositioning themselves. "
        "Their hands rest too casually on their weapons, and their eyes are too alert. "
        "Something isn't right. The way Lyra smiles reminds you of a cat cornering its prey. "
        "You need to decide quickly: trust your instincts or trust her offer."
    );

    // Additional outcomes for dead-end fixes
    StoryNode* guardAcceptance = createStoryNode(11,
        "The captain considers your words carefully, then nods slowly. 'Very well,' he says, "
        "'you may enter. But know that we'll be watching.' As you pass through the gate, "
        "you notice a hooded figure in the shadows taking particular interest in your arrival."
    );

    StoryNode* ghostlyAlliance = createStoryNode(12,
        "The spectral scholar's form brightens with interest. 'Ah, the Crown! Yes... I know much of its power. "
        "But knowledge comes with responsibility. Are you prepared for what you might learn?'"
    );

    StoryNode* merchantDealAccept = createStoryNode(13,
        "Lyra's eyes glitter with satisfaction. 'Excellent choice,' she says. 'My contacts can get you "
        "into the inner city, where the Crown is kept. But first, we need to establish your cover identity. "
        "I have three potential covers prepared: a visiting scholar from the Academy of Mysteries, "
        "a wealthy merchant from the Eastern Kingdoms, or a diplomat from the Northern Realms.'"
    );

    // New nodes for merchant path continuation
    StoryNode* scholarCover = createStoryNode(14,
        "Lyra provides you with forged credentials from the Academy of Mysteries. "
        "'The Shadowmancer often seeks counsel from scholars,' she explains. 'Your supposed research "
        "into celestial artifacts should pique his interest. But be warned - he may test your knowledge. "
        "The Academy's reputation for eccentric brilliance might help explain any... unusual responses.'"
    );

    StoryNode* wealthyMerchantCover = createStoryNode(15,
        "You assume the identity of a merchant prince from the Eastern Kingdoms. Lyra's people outfit you "
        "with exotic goods and detailed trade documents. 'The Shadowmancer's tower requires constant supplies "
        "of rare materials,' she explains. 'Your generous offers of exclusive trade rights should "
        "grant you an audience. Just remember - wealth talks, but too much talk raises suspicion.'"
    );

    StoryNode* diplomatCover = createStoryNode(16,
        "Lyra presents you with diplomatic seals from the Northern Realms. 'The Shadowmancer maintains "
        "a delicate political balance,' she whispers. 'A diplomat bearing news of potential alliances "
        "will catch his attention. The real Northern delegation isn't due for months, which gives us "
        "time. But be careful - one wrong word about Northern politics could expose you.'"
    );

    // Continuation nodes
    StoryNode* innerCity = createStoryNode(17,
        "Your cover identity secures your entry into Eldara's inner city. Here, the architecture grows "
        "more imposing, with buildings that seem to defy gravity. The Shadowmancer's tower looms ahead, "
        "its dark spire wreathed in perpetual storm clouds. Guards bearing his symbol - a crescent moon "
        "wrapped in shadows - patrol the streets with supernatural awareness."
    );

    StoryNode* cityIntrigue = createStoryNode(18,
        "As you navigate the inner city's politics, you discover three potential allies: "
        "the Twilight Society, a group of noble-born mages; the Shadow Watch, the Shadowmancer's "
        "elite guards who might be convinced to turn; and the Veiled Circle, a secret organization "
        "of city merchants with their own plans for the Crown."
    );

    // Add choices to the starting node
    addChoice(start, "Approach the main gate with confidence", mainGate, 6, 0, 0);
    addChoice(start, "Investigate the mysterious aqueduct", secretPath, 0, 7, 0);
    addChoice(start, "Seek opportunity among the merchants", merchantCaravan, 0, 0, 6);

    // Add choices to main gate branch
    addChoice(mainGate, "Stand your ground and demand entry", confrontGuards, 8, 0, 0);
    addChoice(mainGate, "Attempt to bribe the guard", bribeGuards, 0, 0, 7);

    // Add choices to secret path branch
    addChoice(secretPath, "Follow the dark tunnel deeper", magicalEncounter, 0, 8, 0);
    addChoice(secretPath, "Take the path toward daylight", ancientPassage, 6, 0, 0);

    // Add choices to merchant branch
    addChoice(merchantCaravan, "Negotiate with Lyra", merchantDeal, 0, 0, 8);
    addChoice(merchantCaravan, "Keep your guard up", merchantBetrayal, 0, 7, 0);

    // Fix dead ends by adding choices to previously terminal nodes
    // Magical encounter choices
    addChoice(magicalEncounter, "Tell him about the Crown", ghostlyAlliance, 0, 8, 0);
    addChoice(magicalEncounter, "Ask about the Old Kingdom", ancientPassage, 0, 7, 0);
    addChoice(magicalEncounter, "Leave the tunnel", secretPath, 6, 0, 0);

    // Confront guards choices
    addChoice(confrontGuards, "Tell the truth about your quest", guardAcceptance, 0, 0, 8);
    addChoice(confrontGuards, "Try to intimidate them", bribeGuards, 8, 0, 0);
    addChoice(confrontGuards, "Retreat and find another way", secretPath, 6, 0, 0);

    // Merchant deal choices
    addChoice(merchantDeal, "Accept her offer", merchantDealAccept, 0, 0, 8);
    addChoice(merchantDeal, "Negotiate better terms", merchantBetrayal, 0, 7, 0);
    addChoice(merchantDeal, "Decline and leave", mainGate, 0, 0, 6);

    // Merchant betrayal choices
    addChoice(merchantBetrayal, "Fight your way out", confrontGuards, 8, 0, 0);
    addChoice(merchantBetrayal, "Try to bluff", merchantDeal, 0, 0, 8);
    addChoice(merchantBetrayal, "Escape through the crowd", secretPath, 7, 0, 0);

    // Ancient passage choices
    addChoice(ancientPassage, "Explore deeper", magicalEncounter, 0, 8, 0);
    addChoice(ancientPassage, "Follow the light", guardAcceptance, 6, 0, 0);
    addChoice(ancientPassage, "Turn back", secretPath, 0, 0, 6);

    // Ghostly alliance choices
    addChoice(ghostlyAlliance, "Accept his guidance", merchantDealAccept, 0, 8, 0);
    addChoice(ghostlyAlliance, "Question his motives", ancientPassage, 0, 7, 0);
    addChoice(ghostlyAlliance, "Decline and leave", secretPath, 6, 0, 0);

    // MerchantDealAccept choices
    addChoice(merchantDealAccept, "Pose as a scholar", scholarCover, 0, 8, 0);
    addChoice(merchantDealAccept, "Assume merchant identity", wealthyMerchantCover, 0, 0, 8);
    addChoice(merchantDealAccept, "Take diplomatic cover", diplomatCover, 0, 7, 7);

    // Add choices to each cover identity
    addChoice(scholarCover, "Enter the inner city", innerCity, 0, 8, 0);
    addChoice(scholarCover, "Research local politics", cityIntrigue, 0, 7, 0);
    addChoice(scholarCover, "Reconsider approach", merchantDealAccept, 0, 0, 6);

    addChoice(wealthyMerchantCover, "Enter the inner city", innerCity, 0, 0, 8);
    addChoice(wealthyMerchantCover, "Network with locals", cityIntrigue, 0, 0, 7);
    addChoice(wealthyMerchantCover, "Reconsider approach", merchantDealAccept, 0, 6, 0);

    addChoice(diplomatCover, "Enter the inner city", innerCity, 0, 7, 7);
    addChoice(diplomatCover, "Gather intelligence", cityIntrigue, 0, 7, 0);
    addChoice(diplomatCover, "Reconsider approach", merchantDealAccept, 0, 0, 6);

    // Add choices to inner city and intrigue
    StoryNode* twilightSociety = createStoryNode(19,
        "The Twilight Society meets in a floating tower, accessible only by magic. Their leader, "
        "Archmage Venna, sees through your cover immediately but seems amused. 'Another seeker of the Crown? "
        "Interesting. Perhaps we can help each other. The Shadowmancer's power grows unchecked, and "
        "we seek... balance.'"
    );

    StoryNode* shadowWatch = createStoryNode(20,
        "Through careful observation, you identify Captain Raven of the Shadow Watch - one of the few "
        "who remembers serving before the Shadowmancer's rise to power. 'Things were different once,' "
        "she whispers during a secret meeting. 'Some of us remember. Some of us wait. But timing is "
        "everything, and one wrong move could doom us all.'"
    );

    StoryNode* veiledCircle = createStoryNode(21,
        "The Veiled Circle contacts you through a series of subtle signals and coded messages. Their "
        "representative, known only as 'The Voice,' offers a compelling proposition: 'The Crown's power "
        "affects trade routes, market forces, the very flow of wealth. Help us claim it, and we'll "
        "make you richer than the Shadowmancer himself.'"
    );

    addChoice(innerCity, "Approach Twilight Society", twilightSociety, 0, 8, 0);
    addChoice(innerCity, "Contact Shadow Watch", shadowWatch, 0, 0, 8);
    addChoice(innerCity, "Investigate Veiled Circle", veiledCircle, 0, 7, 7);

    addChoice(cityIntrigue, "Join Twilight Society", twilightSociety, 0, 8, 0);
    addChoice(cityIntrigue, "Ally with Shadow Watch", shadowWatch, 7, 0, 7);
    addChoice(cityIntrigue, "Deal with Veiled Circle", veiledCircle, 0, 7, 8);

    // Add choices to new factions
    addChoice(twilightSociety, "Accept magical training", innerCity, 0, 9, 0);
    addChoice(twilightSociety, "Share Crown knowledge", cityIntrigue, 0, 8, 0);
    addChoice(twilightSociety, "Maintain independence", merchantDealAccept, 0, 7, 0);

    addChoice(shadowWatch, "Join their conspiracy", innerCity, 8, 0, 0);
    addChoice(shadowWatch, "Gather guard intel", cityIntrigue, 0, 7, 7);
    addChoice(shadowWatch, "Keep your distance", merchantDealAccept, 6, 0, 0);

    addChoice(veiledCircle, "Accept their resources", innerCity, 0, 0, 9);
    addChoice(veiledCircle, "Negotiate terms", cityIntrigue, 0, 7, 8);
    addChoice(veiledCircle, "Decline carefully", merchantDealAccept, 0, 7, 7);

    return start;
}

// Chapter 3 content
StoryNode* createChapterThree(void) {
    // The Crown's Chamber
    StoryNode* crownChamber = createStoryNode(26,
        "The Shadowmancer's private sanctum lies before you. The circular chamber seems to exist in multiple places at once, "
        "its walls shifting between different planes of reality. The Celestial Crown hovers in the center, "
        "its crystalline form capturing the essence of stars themselves. As you approach, "
        "reality ripples, and three distinct paths manifest before you."
    );

    StoryNode* timePortal = createStoryNode(27,
        "A shimmering portal tears through space-time, showing glimpses of Eldara's past. "
        "You see the city in its prime, when the Crown was first forged. The Shadowmancer stands beside it, "
        "but younger, human, his eyes filled with hope instead of darkness. "
        "'Sometimes,' his present voice echoes, 'to move forward, we must first step back.'"
    );

    StoryNode* celestialAscension = createStoryNode(28,
        "The Crown pulses with stellar energy, responding to your presence. Streams of starlight "
        "spiral around you, offering transcendence. You could claim its power, become something "
        "more than mortal. The Shadowmancer watches with knowing eyes. 'Power changes us,' he warns. "
        "'The question is: will you change it, or will it change you?'"
    );

    StoryNode* shadowMerger = createStoryNode(29,
        "The shadows around the Crown coalesce, forming a bridge between you and the Shadowmancer. "
        "'There is another way,' he offers. 'Not as master and servant, but as equals. "
        "The Crown was never meant for one bearer alone.' The shadows extend toward you, "
        "offering a partnership that could reshape reality itself."
    );

    // Final outcomes
    StoryNode* pastRedemption = createStoryNode(30,
        "You step through the portal, into a moment that changed history. The young Shadowmancer "
        "turns, recognition flickering in his eyes. 'You... I remember this moment. You were there, "
        "will be there, are there.' Time flows like water around you both, and in this nexus of possibility, "
        "you find a way to reshape the Crown's destiny - and Eldara's future."
    );

    StoryNode* divineAscension = createStoryNode(31,
        "The Crown's power flows into you like liquid starlight. Your consciousness expands "
        "beyond mortal limits, touching the very fabric of reality. You see Eldara not just as a city, "
        "but as a nexus of countless possible futures. With this power, you could guide it toward "
        "a new golden age - or transcend this plane entirely."
    );

    StoryNode* dualGuardians = createStoryNode(32,
        "Light and shadow intertwine as you and the Shadowmancer forge a new pact. The Crown "
        "splits and reforms into twin artifacts, each reflecting both darkness and light. "
        "'Balance,' the Shadowmancer smiles, his form becoming more human. 'This was always "
        "the true purpose. Not dominion, but harmony.'"
    );

    // Add final choices
    addChoice(crownChamber, "Enter the time portal", timePortal, 0, 10, 0);
    addChoice(crownChamber, "Embrace the Crown's power", celestialAscension, 0, 9, 0);
    addChoice(crownChamber, "Consider the Shadowmancer's offer", shadowMerger, 0, 0, 10);

    // Time portal outcomes
    addChoice(timePortal, "Change the past", pastRedemption, 0, 10, 0);
    addChoice(timePortal, "Preserve the timeline", celestialAscension, 0, 9, 0);

    // Ascension outcomes
    addChoice(celestialAscension, "Claim godhood", divineAscension, 0, 10, 0);
    addChoice(celestialAscension, "Remain mortal", dualGuardians, 0, 0, 10);

    // Shadow merger outcomes
    addChoice(shadowMerger, "Accept partnership", dualGuardians, 0, 0, 10);
    addChoice(shadowMerger, "Claim the power alone", divineAscension, 0, 10, 0);

    // Epilogue nodes
    StoryNode* epiloguePast = createStoryNode(33,
        "Eldara flourishes under the guidance of two Shadowmancers - one from the future, one from the past. "
        "The Celestial Crown becomes a symbol of wisdom rather than power, its true purpose fulfilled. "
        "Time itself seems to smile upon this outcome, as past and present weave together in perfect harmony. "
        "Your legend lives on in both timelines, a guardian of balance through the ages."
    );

    StoryNode* epilogueAscension = createStoryNode(34,
        "From your celestial throne, you watch over Eldara as it grows into a beacon of enlightenment. "
        "Your presence inspires generations of seekers and scholars, each adding to the city's legacy. "
        "Though you've transcended mortality, you never forget your human journey, and this remembrance "
        "keeps you connected to those you now guide."
    );

    StoryNode* epilogueBalance = createStoryNode(35,
        "Years pass, and Eldara transforms under the guidance of its twin guardians. Light and shadow dance "
        "in perfect balance, creating wonders that draw visitors from across the world. The Celestial Crown, "
        "now split into its dual aspects, serves as a reminder that the greatest power lies not in dominion, "
        "but in the harmony between opposing forces."
    );

    // Connect epilogues
    addChoice(pastRedemption, "Complete your journey", epiloguePast, 0, 0, 0);
    addChoice(divineAscension, "Embrace your destiny", epilogueAscension, 0, 0, 0);
    addChoice(dualGuardians, "Forge the future", epilogueBalance, 0, 0, 0);

    return crownChamber;
}

// Side Quests and Additional Content
StoryNode* createSideQuests(void) {
    // The Alchemist's Request
    StoryNode* alchemistShop = createStoryNode(36,
        "In a narrow alley, you discover 'The Midnight Mortar' - an ancient alchemist's shop. "
        "Through windows clouded with multi-colored smoke, you see shelves lined with glowing potions. "
        "A weathered sign reads: 'Madame Moira's Miraculous Mixtures'. "
        "Something about the shop seems to call to you."
    );

    StoryNode* moiraMeeting = createStoryNode(37,
        "Madame Moira, an elderly woman with kaleidoscope eyes and silver-streaked hair, "
        "studies you intently. 'Ah, you seek the Crown,' she says, sorting through bottles. "
        "'But perhaps we can help each other. I need someone with your... unique qualities. "
        "The Shadowmancer's magic has corrupted the city's ley lines, and my remedies grow weak.'"
    );

    StoryNode* leylineQuest = createStoryNode(38,
        "The city's ancient ley lines pulse beneath your feet, visible only to those who know how to look. "
        "Moira's enchanted map shows three nexus points where the corruption is strongest. "
        "Each requires a different approach to cleanse, and time is of the essence. "
        "The Shadowmancer's influence grows stronger with each passing hour."
    );

    // The Thieves' Guild Plot
    StoryNode* thiefContact = createStoryNode(39,
        "A hooded figure slips you a note in the crowded market: "
        "'We know what you seek. The Nightshade Guild has eyes everywhere. "
        "Meet us at the Rusty Anchor tavern at midnight if you want to learn more. "
        "Come alone, or don't come at all.'"
    );

    StoryNode* guildMeeting = createStoryNode(40,
        "The Rusty Anchor's back room is thick with pipe smoke and secrets. "
        "The Guild's representative, a scarred woman called 'The Raven', gets straight to business. "
        "'The Crown's not just a magical trinket - it's the key to the city's oldest vault. "
        "Help us access it, and we'll help you reach the Shadowmancer's sanctum.'"
    );

    // The Scholar's Discovery
    StoryNode* libraryEncounter = createStoryNode(41,
        "In the Grand Library's restricted section, you meet Master Thaddeus, "
        "a scholar obsessed with Eldara's history. His desk is buried under ancient scrolls. "
        "'The Crown's true purpose!' he whispers excitedly. 'It's not what anyone thinks. "
        "I've found references to something called the Celestial Concordat...'"
    );

    // Resistance Movement
    StoryNode* resistanceContact = createStoryNode(42,
        "A street performer's song carries a coded message. Those who oppose the Shadowmancer "
        "still exist, working from the shadows. Their leader, known only as 'The Dawn', "
        "offers a different path to the Crown - one that could free Eldara from both "
        "its current master and the burden of its ancient magics."
    );

    // Magical Anomalies
    StoryNode* anomalyDiscovery = createStoryNode(43,
        "Strange magical disturbances plague the city's lower districts. "
        "Reality shifts unpredictably - rain falls upward, shadows move against the light, "
        "and citizens report seeing multiple versions of themselves. "
        "These anomalies seem connected to the Crown's growing instability."
    );

    // Add choices to side quests
    addChoice(alchemistShop, "Enter the shop", moiraMeeting, 0, 6, 0);
    addChoice(moiraMeeting, "Help with the ley lines", leylineQuest, 0, 8, 0);

    addChoice(thiefContact, "Attend the meeting", guildMeeting, 7, 0, 0);
    addChoice(guildMeeting, "Investigate the vault", anomalyDiscovery, 8, 0, 0);

    // Ley Line Outcomes
    StoryNode* northNexus = createStoryNode(44,
        "The northern nexus lies beneath the city's oldest temple. "
        "Dark energy writhes around the ancient stones, while ghostly figures "
        "perform endless rituals. The corruption here is deep, but you sense "
        "a pattern in the chaos - a weakness that could be exploited."
    );

    StoryNode* eastNexus = createStoryNode(45,
        "In the merchant district, the eastern nexus pulses beneath a busy marketplace. "
        "The corruption here manifests as subtle manipulation - merchants driven to dishonesty, "
        "customers compelled to ruin. The negative energy feeds on mortal greed, "
        "growing stronger with each tainted transaction."
    );

    StoryNode* westNexus = createStoryNode(46,
        "The western nexus, hidden in the artisan quarter, affects creativity itself. "
        "Artists create works of disturbing beauty, musicians play songs that entrance listeners "
        "in harmful ways. The corruption twists inspiration into obsession, "
        "turning the district's creative energy toward darker purposes."
    );

    // Add nexus choices
    addChoice(leylineQuest, "Cleanse northern nexus", northNexus, 0, 9, 0);
    addChoice(leylineQuest, "Purify eastern nexus", eastNexus, 0, 0, 8);
    addChoice(leylineQuest, "Heal western nexus", westNexus, 0, 7, 0);

    // Resistance Outcomes
    StoryNode* dawnMeeting = createStoryNode(47,
        "The resistance meets in an abandoned temple, its members diverse but united. "
        "The Dawn, a figure wrapped in light-bending cloaks, reveals the truth: "
        "the Shadowmancer was once their leader, before the Crown's power corrupted him. "
        "They seek not to claim the Crown, but to destroy it - if such a thing is possible."
    );

    StoryNode* celestialConcordat = createStoryNode(48,
        "Master Thaddeus's research reveals a shocking truth: the Celestial Crown "
        "was created as part of an ancient pact with beings from beyond the stars. "
        "Its power was meant to protect Eldara from a prophesied calamity. "
        "The Shadowmancer's use of it has weakened this protection - the city is vulnerable."
    );

    // Connect new content to main story
    addChoice(resistanceContact, "Meet the resistance", dawnMeeting, 0, 0, 8);
    addChoice(libraryEncounter, "Research the Concordat", celestialConcordat, 0, 9, 0);

    // Consequences and Rewards
    StoryNode* leylineRestored = createStoryNode(49,
        "As you complete the ley line cleansing ritual, waves of pure energy pulse "
        "through the city. The corruption recedes, and Madame Moira's remedies regain "
        "their potency. More importantly, you've weakened the Shadowmancer's influence "
        "and gained insight into the nature of his power over the Crown."
    );

    StoryNode* vaultSecret = createStoryNode(50,
        "The Guild's information leads you to a hidden chamber beneath the city. "
        "Ancient mechanisms guard a truth that changes everything: the Crown is just "
        "one part of a larger artifact. Its true power can only be unleashed - or destroyed - "
        "when united with its missing pieces, scattered throughout Eldara."
    );

    // Add final outcomes
    addChoice(northNexus, "Complete the ritual", leylineRestored, 0, 10, 0);
    addChoice(guildMeeting, "Find the vault", vaultSecret, 9, 0, 0);

    return alchemistShop;
}

// Enhanced Shadowmancer Path
StoryNode* createShadowmancerPath(void) {
    StoryNode* towerStudy = createStoryNode(69,
        "The Shadowmancer's private study reveals the complexity of his character. "
        "Journals detail his transformation from idealistic mage to power-wielding sorcerer. "
        "Star charts and prophecies cover the walls, all pointing to an impending celestial event. "
        "Three aspects of his research draw your attention, each offering different insights."
    );

    StoryNode* personalJournals = createStoryNode(70,
        "His early journals paint a different picture: a young mage determined to protect Eldara. "
        "'The Crown chooses its bearer,' he wrote, 'but at what cost? I feel its power changing me, "
        "yet I cannot release it. The threat looms closer each day, and only the Crown stands between "
        "our world and the void.' The final entries grow increasingly paranoid and desperate."
    );

    StoryNode* celestialResearch = createStoryNode(71,
        "The astronomical calculations are precise and troubling. They show a convergence of stars "
        "that occurs once every millennium. 'The Celestial Crown was forged during the last alignment,' "
        "his notes read. 'Its power peaks when the stars align, but so does the ancient threat. "
        "The barrier between worlds grows thin, and They push against it, seeking entry.'"
    );

    StoryNode* shadowTheory = createStoryNode(72,
        "His work on shadow magic suggests a revolutionary theory: shadows aren't absence of light, "
        "but glimpses of other realities bleeding through. The Crown doesn't create power, it channels it "
        "from these alternate worlds. 'Every shadow cast in Eldara is a window,' he writes, "
        "'and I have learned to open them. But what else might slip through?'"
    );

    // Resistance Enhancement
    StoryNode* resistanceBase = createStoryNode(73,
        "The resistance headquarters lies within a pocket dimension, a space between shadows. "
        "Here, you meet the core members: the Lightweaver, master of illusion; the Truthseer, "
        "keeper of histories; and the Voidwalker, who maps the spaces between realities. "
        "Each offers a different perspective on the Crown's true nature."
    );

    StoryNode* lightweaverPath = createStoryNode(74,
        "The Lightweaver reveals how the Crown bends not just shadow, but reality itself. "
        "'Its power lies in perception,' she explains, weaving illusions of possible futures. "
        "'The Shadowmancer doesn't control the Crown - it controls him, showing him only shadows "
        "of truth. But together, we might pierce this veil of deception.'"
    );

    StoryNode* truthseerPath = createStoryNode(75,
        "The Truthseer's chamber is lined with memory crystals, each containing a fragment of history. "
        "'The Crown has had many bearers,' he reveals, 'each believing they were the first. But its true "
        "purpose was never dominion - it was created as a key, a seal, and a weapon. The question is: "
        "against what?'"
    );

    StoryNode* voidwalkerPath = createStoryNode(76,
        "The Voidwalker's insights are both illuminating and terrifying. Through her arts, you glimpse "
        "the spaces between worlds where ancient entities dwell. 'The Crown is a lighthouse,' she explains, "
        "'keeping darker powers at bay. The Shadowmancer maintains this vigil, though it consumes him. "
        "We must find another way, or risk losing everything to the void.'"
    );

    // Celestial Mysteries
    StoryNode* starChamber = createStoryNode(77,
        "Atop the highest tower, the Shadowmancer maintains a celestial observatory. "
        "The domed chamber tracks the movements of stars and stranger things. "
        "Here, the boundaries between worlds are thinnest, and the Crown's power manifests "
        "in its purest form. Three phenomena demand attention, each offering unique insights."
    );

    StoryNode* constellationGate = createStoryNode(78,
        "A pattern of stars forms a gateway to other realms. Through it, you witness "
        "countless versions of Eldara - some radiant with power, others dark and empty. "
        "The Crown's light holds these realities apart, preventing them from collapsing "
        "into chaos. But the barrier grows weaker with each passing night."
    );

    StoryNode* voidWindow = createStoryNode(79,
        "A section of the observatory wall simply... ends. Beyond it lies the void itself, "
        "held at bay by the Crown's power. Ancient entities press against this barrier, "
        "their forms defying mortal comprehension. You begin to understand the Shadowmancer's "
        "obsession - his burden is greater than anyone suspected."
    );

    StoryNode* timeNexus = createStoryNode(80,
        "In one corner, time itself pools like liquid, showing moments from Eldara's past "
        "and possible futures. You see the Crown's creation, its many bearers, and glimpses "
        "of what might come. The Shadowmancer appears in many of these visions, his role "
        "more complex than simply villain or guardian."
    );

    // Add choices to Shadowmancer path
    addChoice(towerStudy, "Read personal journals", personalJournals, 0, 8, 0);
    addChoice(towerStudy, "Study celestial research", celestialResearch, 0, 9, 0);
    addChoice(towerStudy, "Examine shadow theory", shadowTheory, 0, 8, 0);

    // Add choices to resistance path
    addChoice(resistanceBase, "Learn from Lightweaver", lightweaverPath, 0, 0, 8);
    addChoice(resistanceBase, "Consult Truthseer", truthseerPath, 0, 9, 0);
    addChoice(resistanceBase, "Follow Voidwalker", voidwalkerPath, 0, 8, 0);

    // Add choices to celestial path
    addChoice(starChamber, "Explore constellation gate", constellationGate, 0, 9, 0);
    addChoice(starChamber, "Study void window", voidWindow, 0, 8, 0);
    addChoice(starChamber, "Examine time nexus", timeNexus, 0, 9, 0);

    // Final revelations
    StoryNode* cosmicTruth = createStoryNode(81,
        "The pieces align, revealing a truth both wonderful and terrible. The Crown, the Shadowmancer, "
        "the very fabric of Eldara - all are part of an ancient system maintaining reality itself. "
        "The coming celestial alignment doesn't just empower the Crown; it's a moment when all possible "
        "futures converge, and the fate of not just Eldara, but all realities, hangs in the balance."
    );

    StoryNode* shadowPact = createStoryNode(82,
        "Understanding dawns: the Shadowmancer's transformation was a calculated sacrifice. "
        "By becoming a creature of shadow, he gained the power to maintain the barriers between worlds. "
        "His apparent tyranny masks a desperate vigil against forces that would unmake reality. "
        "Now you must decide: maintain this system, or risk everything to change it."
    );

    StoryNode* timeChoice = createStoryNode(83,
        "The time nexus offers one last revelation: a way to reshape the past without unraveling reality. "
        "You could prevent the Crown's creation, but doing so would require rewriting history itself. "
        "The consequences would cascade through time, creating a new world - for better or worse. "
        "The choice, and the responsibility, rests with you."
    );

    // Connect final revelations
    addChoice(personalJournals, "Grasp the cosmic truth", cosmicTruth, 0, 10, 0);
    addChoice(voidwalkerPath, "Understand the shadow pact", shadowPact, 0, 0, 10);
    addChoice(timeNexus, "Consider changing history", timeChoice, 0, 10, 0);

    return towerStudy;
}

StoryNode* initializeStory(void) {
    return createChapterOne();
}

void cleanupStory(StoryNode* root) {
    if (!root) return;
    
    for (int i = 0; i < root->numChoices; i++) {
        cleanupStory(root->nextNodes[i]);
    }
    
    free(root);
} 