Class MyContainer
    with max_capacity,
        holding [counter i;
            objectloop(i in self)   {
                counter = counter + i.mass;
                print"counter = ",counter,".^";
                print"added ",i.mass," to counter.^";
            }
            print"final counter = ",counter,"^";
            return counter;
        ],
        before [ x;
            receive:
                x = self.holding();
                if (x + noun.mass > self.max_capacity) "There's not enough room in ",(the)self,".";
        ],
    has container;

Class SwipeCard
    with 
        mass 1,
        before [;
            Swipe:
                if (noun == self && real_location == hallway_b2) {
                    print"(through the card reader)^";
                    print"You hear a 'click' from the door and the light on the reader changes to green.^";
                    give central_supply_door open;
                    give central_supply_door ~locked;
                    give card_reader on;
                    StopTimer(card_reader);
                    StartTimer(card_reader ,3);
                    rtrue;
                }
                if (noun == self && second == card_reader) {
                    print"You hear a 'click' from the door and the light on the reader changes to green.^";
                    give central_supply_door open;
                    give central_supply_door ~locked;
                    give card_reader on;
                    StopTimer(card_reader);
                    StartTimer(card_reader ,3);
                } 
            Insert:
                if (noun == self && second == card_reader) {
                    print"You hear a 'click' from the door and the light on the reader changes to green.^";
                    give central_supply_door open;
                    give central_supply_door ~locked;
                    give card_reader on;
                    StopTimer(card_reader);
                    StartTimer(card_reader ,3);
                }  
        ];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SwipeCard swipe_card "security card" security_desk
    with name 'card' 'swipe' 'security',
        description "It's a white plastic rectangular swipe card. ",
        mass 1,
        before [;
            take:
            if (self in security_desk && buzz in security_chair) "Buzz stops you. ~Hands off, little lady.~";
        ],
        has item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object flashlight "flashlight" morgue
    with name 'flashlight' 'light' 'torch' 'black' 'plastic',
        description [;
            print"It's a standard black plastic flashlight, ";
            if(self has on) "currently on. "; "currently off. ";
        ],
        mass 6,
        after [;
            switchon:
                give self light;
            switchoff:
                give self ~ light;
        ],
    has switchable valuable item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Global walkman_playing = false;
Object walkman "walkman" morgue
    with name 'walkman' 'player' 'compartment',
        description [ obj;
            print"It's a silver plastic and metal tape player. It's slightly smaller than a thick 
            paperback book. There's a compartment in the side, currently ";
            if(self has open) print"open, "; else print "closed, ";
            print"where the tape is inserted and several chunky black buttons protrude from the edge 
            including 'play', 'stop', 'fast fwd', 'rewind', 'eject'. ";
            if(headphones_plugged) print"There's a pair of headphones currently plugged into the player. ";
            if(self has open) 
            {
                objectloop(obj in self)
                {
                    if (obj ofclass Tape) "A cassette tape sits in the open compartment. "; 
                }
                "The open compartment is empty. ";
            }
            objectloop(obj in self)
            {
                if (obj ofclass Tape) print"Through the little window you can see a cassette tape inside ";      
            }
            if(walkman_playing) "and that the little rotors are turning. "; else "and that the little 
            rotors are motionless. ";
        ],
        before [;
            open:
                <<push eject_button>>;
        ],
        invent [;
            if (inventory_stage == 2) rtrue;
        ],
        tape_is_loaded  [ obj;
            objectloop(obj in self)
            {
                if (obj ofclass Tape) return obj;
            }
                rfalse;
        ],
        describe [;
            "^You see a walkman here.";
        ],
        mass 6, 
    has container transparent openable item;

Object eject_button "eject button" walkman
    with name 'eject' 'button',
        description"It's a chunky black button with the 'eject' symbol on the top. ",
        before [;
            push:
                if(walkman notin player) 
                {
                    move walkman to player;
                    print"(first taking the walkman)^";
                }
                if(walkman has open) "The walkman is already open. ";
                walkman_playing = false;
                give walkman open;
                "You press the 'eject' button and the tape compartment springs open with a ~clatter~.";
        ],
    has scenery;

Object play_button "play button" walkman
    with name 'play' 'button',
        description"It's a chunky black button with a 'play' arrow on the top. ",
        before [ obj;
            push:
                if(walkman notin player) 
                {
                    move walkman to player;
                    print"(first taking the walkman)^";
                }
                if(walkman has open) "You should close the tape compartment first. ";
                if(walkman_playing) "The walkman is already playing. ";
                obj = walkman.tape_is_loaded();
                if(obj)
                {
                    obj.press_play();
                } 
                rtrue;
        ],
    has scenery;   

Object stop_button "stop button" walkman
    with name 'stop' 'button',
        description"It's a chunky black button with the 'stop' square on the top. ",
        before [;
            push:
                if(walkman notin player) 
                {
                    move walkman to player;
                    print"(first taking the walkman)^";
                }
                if(~~walkman_playing) "It's already stopped. ";
                walkman_playing = false;
                "You press the button and the 'play' button disengages with a ~clunk~. ";
        ],
    has scenery;   

Object fast_forward_button "fast forward button" walkman
    with name 'fast' 'forward' 'button',
        description"It's a chunky black button with two 'FF' arrows on the top. ",
        before [ obj;
            push:
                if(walkman notin player) 
                {
                    move walkman to player;
                    print"(first taking the walkman)^";
                }
                obj = walkman.tape_is_loaded();
                if(obj)
                {
                    obj.fast_forward();
                } 
                else
                {
                    walkman_playing = false;
                    "You press the button down and with a ~whir~ the little spools spin rapidly. After a moment you 
                    release the button.";
                }
                rtrue;    
        ],
    has scenery;  

Object rewind_button "rewind button" walkman
    with name 'rewind' 'button',
        description"It's a chunky black button with two backwards 'rewind' arrows on the top.",
        before [ obj;
            push:
                if(walkman notin player) 
                {
                    move walkman to player;
                    print"(first taking the walkman)^";
                }
                obj = walkman.tape_is_loaded();
                if(obj)
                {
                    obj.rewind();
                } 
                else
                {
                    walkman_playing = false;
                    "You press the button down and with a ~whir~ the little spools spin rapidly backwards. After a moment you 
                    release the button.";
                }
                rtrue;    
        ],
    has scenery;              

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object headphones "headphones" walkman 
    with name 'headphones' 'headphone' 'phones',
        description [;
            print"They're comprised of a thin plastic curved band ending at two orange puffy ear pieces. ";
            if (headphones_plugged) "They're currently plugged into the walkman. "; else "";
        ],
        before  [;
            take:
                if (self in walkman) 
                {
                    move self to real_location;
                    <take walkman>;
                }
            drop:
                if (headphones_plugged) "You need to unplug them from the walkman first. ";            
            plugin:
                if(headphones_plugged) "They're already plugged in. ";
                if(walkman in player) 
                {
                    print"(into the walkman)^";
                    move self to walkman;
                    headphones_plugged = true;
                    "You plug the headphones into the walkman. ";
                }
                "You should specify what you want to plug them into. ";
            pluginto:
                if(self in walkman) "They're already plugged in. ";
                if(second == walkman) 
                {
                    if(walkman in self) 
                    {
                        print"(into the walkman)^";
                        move self to walkman;
                        headphones_plugged = true;
                        "You plug the headphones into the walkman. ";
                    }
                    else
                    {
                        print"(first taking the walkman)^";
                        move self to walkman;
                        headphones_plugged = true;
                        "You plug the headphones into the walkman. ";
                    }
                }
            unplug:
                if (~~headphones_plugged) "They're already unplugged. ";
                headphones_plugged = false;
                move self to parent(walkman);
                "You unplug the headphones from the walkman. ";
        ],
        mass 2,
    has clothing pluralname item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Constant SIDE_A = 0;
Constant SIDE_B = 1;
Constant SIDE_START = 0;
Constant FIRST_TRACK = 1;
Constant SIDE_END = 7;

Class Tape 
    with
        is_in_player [obj;
        if(obj in walkman) rtrue; rfalse;
    ],
    mass 2,
    current_side,
    current_track,
    fast_forward [ i;
        walkman_playing = false;
        if (self.current_track == SIDE_END) "The tape seems to be at the end of the side. ";
        i = self.current_track;
        i++;
        self.current_track = i;
        print"in FF new track = ",i,"^";
        if (self.current_track == SIDE_END) "You press the button down and with a ~whir~ the little spools spin rapidly.
            With a ~click~ you reach the end of the side. "; 
        "You press the button down and with a ~whir~ the little spools spin rapidly. After a moment you 
        release the button. ";
    ],
    press_play [;
        if (self.current_track == SIDE_END) "You press the button but you seem to be at the end of this side of the tape. ";
        if (self.current_track == SIDE_START) self.current_track = FIRST_TRACK;
        print"With a satisfying ~click~ the play button engages.^";
        walkman_playing = true;
        ],
    play [;
        if (self.current_track == SIDE_END) "The tape seems to be at the end of the side. ";
        if (self.current_track == SIDE_START) self.current_track = FIRST_TRACK;
        self.playback();
    ],
    before [ x;
        flip:
            print"flipping ",(name)self,"^";
            walkman_playing = false;
            if (self.current_side == SIDE_A) self.current_side = SIDE_B; else self.current_side = SIDE_A;
            print"now side = ",self.current_side,"^";
            x = self.current_track;
            print"current track = ",x,"^";
            give walkman ~open;
            print"You pop out the tape, flip it over, put it back in, and snap the cassette compartment closed.^"; 
            if (x == SIDE_END) { self.current_track = FIRST_TRACK; rtrue; }
            if (x == SIDE_START) { self.current_track = SIDE_END; rtrue; }
            x = SIDE_END - x;
            self.current_track = x;
            print_ret"now current track = ",self.current_track,".";
        ],
    rewind [ i;
        walkman_playing = false;
        if (self.current_track == SIDE_START) { self.current_track = FIRST_TRACK; "The tape seems to already be rewound to the beginning. "; }
        i = self.current_track;
        i--;
        self.current_track = i;
        if (self.current_track == SIDE_START) "You press the button down and with a ~whir~ the little spools spin rapidly
            backwards. With a ~click~ you reach the beginning of the side. "; 
        "You press the button down and with a ~whir~ the little spools spin rapidly backwards. After a moment you 
        release the button. ";
    ];

Tape mixtape "casette tape with a yellow label" walkman
    with name 'mix' 'mixtape' 'tape' 'cassette' 'yellow',
        description"It's a cassette tape with a yellow label. You see ~mixtape~ handwritten on it.",
        current_side SIDE_A,
        current_track FIRST_TRACK,
        playback [;
            print"^Through the headphones you hear ";
                if (self.current_side == SIDE_A)
                {
                    switch (self.current_track) 
                    {
                    1:  print"~New Song~ by Howard Jones";
                    2:  print"~Mexican Radio~ by Wall of Voodoo";
                    3:  print"~Major Tom (Coming Home)~ by Peter Schilling";
                    4:  print"~Don't Cry~ by Asia";
                    5:  print"~Jeopardy~ by The Greg Kihn Band";
                    6:  print"~Electric Avenue~ by Eddy Grant";
                    }
                }
                if (self.current_side == SIDE_B)
                {
                    switch (self.current_track) 
                    {
                    1:  print"~Overkill~ by Men At Work";
                    2:  print"~I Melt With You~ by Modern English";
                    3:  print"~Love is a Stranger~ by Eurythmics";
                    4:  print"~Radio Free Europe~ by R.E.M.";
                    5:  print"~In a Big Country~ by Big Country";
                    6:  print"~Breaking the Law~ by Judas Priest";
                    }
                }
                ".";
        ],
        each_turn [;
            if(self in walkman && walkman_playing && walkman in player && headphones has worn && headphones_plugged == true) self.playback();
        ],
        has item;

Tape jorry_tape "cassette tape with a green label" morgue
    with name 'tape' 'cassette' 'green',
        description"It's a cassette tape with a greenlabel. Written on the label you see ~S.J.~",
        current_side SIDE_A,
        current_track FIRST_TRACK,
        play [;
            print"^Through the headphones you hear ";
                if (self.current_side == SIDE_A)
                {
                    switch (self.current_track) 
                    {
                    1: jorry_confession();
                    2, 3, 4, 5, 6:  print"empty tape hissing";
                    }
                }
                if (self.current_side == SIDE_B)
                {
                    switch (self.current_track) 
                    {
                    1, 2, 3, 4, 5, 6:  print"empty tape hissing";
                    }
                }
                ".";
        ],
    has item;

[ jorry_confession ;
    jorry_tape.current_track = 2;
    print"the sounds of a tape-recorder motor and then a voice, distorted and speaking a bit too excitedly
    into the microphone.^^~Hello, my name is Sid Jorry. I am CFO of St. Asterix hospital. I have mailed this to you 
    because I have information you may find interesting pertaining to possible financial misdealings perpetrated by 
    Dr. Walt Northrup, CEO of the hospital.~^^
    He clears his throat awkwardly and you hear paper rustling.^^
    ~It seems the good doctor has been siphoning money from the hospital with the expectation that it would fail. At 
    the same time, Dr. Northrup has become a shadow investor in the company that owns the hospital building and grounds.~^^
    He pauses and you hear what sounds like drinking from a glass.^^
    ~The hospital is likely worth much more as real estate than as a hospital. I have records of all of this in a
    ledger locked in the safe in my office. The combination is 23 right, 18 left, 7 right.~^^
    You hear a heavy ~thump~ and a cough.^^
    ~Now, I'm not feeling well and I think I need my nitroglycerin tablets. I certainly won't be seeking care under Northrup's nose,
    of course.~^^Then there's a loud ~click~ and empty hissing. You press 'stop'";
    rtrue;
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object shrimp_bowl "bowl" security_desk
    with name 'blue' 'bowl',
        description [;
            print"It's a blue bowl, ";
            if (children(self) == 0)    "currently empty. "; else print"currently holding ", (a)(child(self)),".^";
        ],
        mass 5,
        describe [;
            if (self in security_desk) rtrue;
        ],
        before [;
            take:
                if (buzz in security_chair) "Buzz, irritated, stops you. ~Can I help you?~";
            receive:
                if (buzz in security_chair) "Buzz, irritated, stops you. ~Can I help you?~";
        ],
        after [;
                take:
                    give self ~concealed;
        ],
    has container open item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object cocktail_shrimp "bowlfull of shrimp" shrimp_bowl
    with name 'cocktail' 'shrimp',
        description "It's a bowlfull of cocktail shrimp. ",
        mass 5,
        before [;
                take:
                    if (buzz in security_chair) "Buzz, stops you. 
                    ~Excuse me. Would you mind keeping your paws out 
                    of my lunch?~"; else "They're too slippery and messy to handle. ";
        ],
    has static;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object shrimp "rotten shrimp" 
    with name 'rotten' 'putrid' 'nasty' 'shrimp',
        description "It's a single boiled shrimp, left behind and well past its expiration date. ",
        mass 1,
        before [;
            smell:
                "It's fairly rank. ";
            eat:
                "You try but just can't do it. ";
        ],
        describe [; if(self in garbage_can) rtrue;],
        has edible item;



