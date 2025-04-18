Class Item
    with 
    invent [ ;
        if (inventory_stage == 2)
        {
            if(balloon.tied_to == self) print" (to which a balloon is tied)";
        }
    ],
    before [ ;
        take:
        if (balloon.tied_to == self)
        {
            move balloon to player;
            scope_modified = true;
            <<untie balloon self>>;
        }

        insert:
        if (self == ladder or folding_chair && second ofclass DumbwaiterShaft ) print_ret(The)noun," won't fit in the dumbwaiter shaft. ";
        if (second ofclass DumbwaiterShaft && real_location ~= sub_basement_01)
        {
            if (balloon.tied_to == self)
            {
                move balloon to sub_basement_dumbwaiter;
                "You put ",(the)self," in the dumbwaiter. It plummets downward through the shaft trailing the helium 
                balloon behind it. ";
            }
            move self to sub_basement_dumbwaiter;
            "You put ",(the)self," in the dumbwaiter. It plummets downward through the shaft. ";
        } 
        if (balloon.tied_to == self)
        {
            move balloon to second;
            scope_modified = true;
            "You put ",(the)self," with attached balloon into ",(the)second,".";
        }

        untie:
        if (balloon.tied_to ~= self) print_ret(The)self," isn't tied to the balloon. ";
        <<untie balloon self>>;

        tie:
        <<tie second self>>;
    ],
    after [;
        examine:
        if (balloon.tied_to == self) "There's a helium ballon tied to it currently. ";
    ];

Class Floatable
    with 
        before [;
            insert:
            if (self == balloon.tied_to && second ofclass DumbwaiterShaft)
            {
                move balloon to second_floor_dumbwaiter;
                if (real_location == hallway_2_2) "You put ",(the)self," in the dumbwaiter. It floats gently upward suspended 
                from the balloon and bounces on the roof of the shaft here. ";
                "You put ",(the)self," in the dumbwaiter. It rises up and out of sight, suspended from the helium balloon. ";
            }
            ];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object swipe_card "security card" security_desk
    with name 'card' 'swipe' 'security',
        description "It's a white plastic rectangular swipe card. ",
        mass 1,
        before [;
            take:
            if (self in security_desk && buzz in security_chair) "Buzz stops you. ~Hands off, little lady.~";
            swipe:
            if (real_location == service_elevator) 
            {
                print"(through the card reader)^";
                if (service_elevator_active) "Swiping the card again doesn't seem to have any additional effect. ";
                service_elevator_call_level = -1;
                StartDaemon(service_elevator_daemon);
                service_elevator_active = true;
                give card_reader on;
                StopTimer(service_close_door_timer);
                if (service_elevator_door has open) { give service_elevator_door ~open; }
                print(string)ELEVATOR_PROSE; 
                rtrue; 
            }
            swipethrough:
            if (second == card_reader) 
            {
                if (service_elevator_active) "Swiping the card again doesn't seem to have any additional effect. ";
                service_elevator_call_level = -1;
                StartDaemon(service_elevator_daemon);
                service_elevator_active = true;
                give card_reader on;
                StopTimer(service_close_door_timer);
                if (service_elevator_door has open) { give service_elevator_door ~open; }
                print(string)ELEVATOR_PROSE; 
                rtrue; 
            }
        ],
        after [;
            take:
            Achieved(1);
            SetFlag(F_HAS_SWIPE_CARD);
        ],
        class Floatable Item;       

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object dummy_headphones "headphones" 
    with
        name 'headphones' 'phones',
        article "a pair of",
        mass 2,
        description "You see two poffy orange ear pieces at the end of a round strip of metal. ",
        before [  ;
            plugin:
            if (dummy_headphones in player)
            {
                print"(into the walkman)^";
            }
            if (self has worn) { give walkman worn; }  else { give walkman ~worn; }
            remove dummy_headphones;
            ClearFlag(F_HEADPHONES_ARE_UNPLUGGED);
            "You plug the headphones into the walkman. ";
        ]
        class Item
    has clothing;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object walkman "your walkman"
    with 
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = Nextword();
            if (w1 == 'compartment') {self.id = 2; return 1; }
            if (w1 == 'tape' && w2 == 'compartment') {self.id = 2; return 1; }
            if (w1 == 'my' && w2 == 'walkman') { self.id = 0; return 2; }
            if (w1 == 'walkman' or 'player') { self.id = 0; return 1; }
            if (w1 == 'headphones' or 'phones') { self.id = 1; return 1; }
        ],
        mass 7,
        id, !0 = walkman, 1 = headphones
        description [ obj;
            if (self.id == 0)
            {
                print"It's a walkman with headphones. The tape compartment in the side is 
                currently ";
                if(self has open) print "open. "; else print"closed. ";
                print"Several chunky black buttons protrude from the side including 'play', 'stop', 
                'f-fwd', 'rewind', and 'eject'. ";
                if (self has open)
                {
                    objectloop (obj in self)
                    {
                        if (obj ofclass tape) "A cassette tape sits in the open compartment. ";
                    }
                    "The open compartment is empty. ";
                }
                objectloop (obj in self)
                {
                    if (obj ofclass Tape) print"Through the little window you can see a cassette tape inside "; 
                }
                if(walkman_playing == true) "and that the little rotors are turning. "; "and that the little 
                rotors are motionless. ";
            }
            if (self.id == 1)
            {
                "You see two poofy orange ear pieces at the end of a round strip of metal. They're plugged into 
                the walkman. ";
            }
            if (self.id == 2)
            {
                print"The compartment is currently ";
                if(self has open) print "open. "; else print"closed. ";
                if (self has open)
                {
                    objectloop (obj in self)
                    {
                        if (obj ofclass tape) "A cassette tape sits in the open compartment. ";
                    }
                    "The open compartment is empty. ";
                }
            }
        ],
        react_before [ ;
            take:
            if (noun in self && noun ofclass Tape)
            {
                if (self hasnt open) <push wm_eject_button>;    
            }
        ],
        before [ ;
            take:
            if(FlagIsSet(F_FREDDY_ASLEEP))
            {
                print"You purloin your walkman from the sleeping security guard.^^Freddy stirs and wakes in a daze.
                ^^~Dude, what happened? Man, did I stroke out grooving to the Aussies?~^"; 
                ClearFlag(F_FREDDY_ASLEEP);
                SetFlag(F_FREDDY_WALKMAN_SHY);
                StartDaemon(freddy);
                move walkman to player;
                scope_modified = true;
                rtrue;
            }
            open:
                if(self.id ~= 1) <<push wm_eject_button>>; "You can't open the headphones. ";
            close:
                if(self.id == 1) "You can't close the headphones. ";
            receive:
            if(self.id == 1) "You can't put things in the headphones. ";
            if (children(self) > 5 ) "There's already a tape in the walkman. ";
            if (~~noun ofclass Tape) "You can't put ",(the)noun," in the walkman. ";

            unplug:
            if (walkman has worn) 
            {
                move dummy_headphones to parent(walkman);
                give dummy_headphones worn;
                give walkman ~worn;
            }
            move dummy_headphones to parent(walkman);
            ClearFlag(F_WALKMAN_BLOCKING);
            SetFlag(F_HEADPHONES_ARE_UNPLUGGED);
            "You pull the headphone plug out of the audio jack. ";

            plugin, pluginto:
            "The headphones are already plugged into the walkman. ";
            take:
            if (FlagIsSet(F_FREDDY_ASLEEP)) move self to parent(freddy);
        ],
        invent [;
            if (inventory_stage == 2) 
            {
                if (self has worn)
                {
                    print" (worn)";
                }
                rtrue;
            }
        ],
        tape_is_loaded  [ obj;
            objectloop(obj in self)
            {
                if (obj ofclass Tape) return obj;
            }
                rfalse;
        ],
        each_turn [ obj;
            obj = walkman.tape_is_loaded();
                if(obj && walkman_playing == true)
                {
                    if (obj.tape_advance == ADVANCE_TIMER)
                    {
                        obj.advance();
                        obj.tape_advance = 0;
                        rtrue;
                    }
                    obj.tape_advance++;
                }   
        ],
    class Item
    has clothing container proper transparent openable;

Object wm_eject_button "walkman eject button" walkman
    with
        parse_name [ w1 w2 w3;
            w1 = NextWord();
            w2 = NextWord();
            w3 = NextWord();
            if (w1 == 'walkman' && w2 == 'eject' && w3 == 'button') return 3;
            if (w1 == 'walkman' && w2 == 'eject') return 2;
            if (w1 == 'eject' && w2 == 'button') return 2;
            if (w1 == 'eject') return 1;
            if (w1 == 'button') return 1;
        ],
        article "an",
        description"It's a chunky black button with the 'eject' symbol on the top. ",
        before [;
            push:
            if (walkman has open) "The walkman is already open. ";
            walkman_playing = false;
            ClearFlag(F_WALKMAN_BLOCKING);
            give walkman open;
            print"You press the 'eject' button and the tape compartment springs open with a ~clatter~.^";
            if(walkman in freddy) freddy_wakes_up();
            rtrue;
        ],
    has scenery;

Object wm_play_button "walkman play button" walkman
    with
        parse_name [ w1 w2 w3;
            w1 = NextWord();
            w2 = NextWord();
            w3 = NextWord();
            if (w1 == 'walkman' && w2 == 'play' && w3 == 'button') return 3;
            if (w1 == 'walkman' && w2 == 'play') return 2;
            if (w1 == 'play' && w2 == 'button') return 2;
            if (w1 == 'play') return 1;
            if (w1 == 'button') return 1;
        ],
        description"It's a chunky black button with a 'play' arrow on the top. ",
        before [ obj;
            push:
                if(walkman has open) "You should close the tape compartment first. ";
                if(walkman_playing) "The walkman is already playing. ";
                obj = walkman.tape_is_loaded();
                if(obj)
                {
                    obj.press_play(walkman);
                    rtrue;
                } 
            walkman_playing = true;
            "With a satisfying ~click~ the play button engages.";
        ],
    has scenery; 

Object wm_stop_button "walkman stop button" walkman
    with
        parse_name [ w1 w2 w3;
            w1 = NextWord();
            w2 = NextWord();
            w3 = NextWord();
            if (w1 == 'walkman' && w2 == 'stop' && w3 == 'button') return 3;
            if (w1 == 'walkman' && w2 == 'stop') return 2;
            if (w1 == 'stop' && w2 == 'button') return 2;
            if (w1 == 'stop') return 1;
            if (w1 == 'button') return 1;
        ],
        description"It's a chunky black button with the 'stop' square on the top. ",
        before [;
            push:
            if(~~walkman_playing) "It's already stopped. ";
            walkman_playing = false;
            ClearFlag(F_WALKMAN_BLOCKING);
            print"You press the button and the 'play' button disengages with a ~clunk~.^";
            if(walkman in freddy) freddy_wakes_up();
            rtrue;
        ],
    has scenery;  

Object wm_fast_forward_button "walkman fast-forward button" walkman
    with
        parse_name [ w1 w2 w3;
            w1 = NextWord();
            w2 = NextWord();
            w3 = NextWord();
            if (w1 == 'walkman' && w2 == 'fast-forward' or 'fast' && w3 == 'button') return 3;
            if (w1 == 'walkman' && w2 == 'fast' or 'forward') return 2;
            if (w1 == 'fast' && w2 == 'forward') return 2;
            if (w1 == 'fast' or 'forward' && w2 == 'button') return 2;
            if (w1 == 'fast' or 'forward') return 1;
            if (w1 == 'ffwd' or 'forward' or 'fastforward') return 1;
            if (w1 == 'button') return 1;
        ],
        description"It's a chunky black button with two 'FF' arrows on the top. ",
        before [ obj;
            push:
                obj = walkman.tape_is_loaded();
                if(obj)
                {
                    obj.fast_forward(walkman);
                    if(walkman in freddy) freddy_wakes_up();
                } 
                else
                {
                    walkman_playing = false;
                    print"You press the button down and with a ~whir~ the little spools spin rapidly. After a moment you 
                    release the button.^";
                }
                rtrue;    
        ],
    has scenery; 

Object wm_rewind_button "walkman rewind button" walkman
    with
        parse_name [ w1 w2 w3;
            w1 = NextWord();
            w2 = NextWord();
            w3 = NextWord();
            if (w1 == 'walkman' && w2 == 'rewind' && w3 == 'button') return 3;
            if (w1 == 'walkman' && w2 == 'rewind') return 2;
            if (w1 == 'rewind' && w2 == 'button') return 2;
            if (w1 == 'rewind') return 1;
            if (w1 == 'button') return 1;
        ],
        description"It's a chunky black button with two backwards 'rewind' arrows on the top.",
        before [ obj;
            push:
                obj = walkman.tape_is_loaded();
                if(obj)
                {
                    obj.rewind(walkman);
                    if(walkman in freddy) freddy_wakes_up();
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
Constant SIDE_A = 0;
Constant SIDE_B = 1;
Constant SIDE_START = 0;
Constant FIRST_TRACK = 1;
Constant SIDE_END = 7;
Constant THRILLER_SIDE_END = 6;
Constant ADVANCE_TIMER = 4;

Class Tape 
    with
    mass 2,
    current_side,
    current_track,
    tape_advance 0,
    fast_forward [ i;
        walkman_playing = false;
        ClearFlag(F_WALKMAN_BLOCKING);
        self.tape_advance = 0;
        if (self ~= thriller && self.current_track == SIDE_END) "The tape seems to be at the end of the side. ";
        if (self == thriller && self.current_track == THRILLER_SIDE_END) "The tape seems to be at the end of the side. ";
        i = self.current_track;
        i++;
        self.current_track = i;
        if (self ~= thriller && self.current_track == SIDE_END) "You press the button down and with a ~whir~ the little spools spin rapidly.
            With a ~click~ you reach the end of the side. "; 
        if (self == thriller && self.current_track == THRILLER_SIDE_END) "You press the button down and with a ~whir~ the little spools spin rapidly.
            With a ~click~ you reach the end of the side. "; 
        "You press the button down and with a ~whir~ the little spools spin rapidly. After a moment you 
        release the button. ";
    ],
    press_play [ ;
        if (self ~= thriller && self.current_track == SIDE_END) "You press the button but you seem to be at the end of this side of the tape. ";
        if (self == thriller && self.current_track == THRILLER_SIDE_END) "You press the button but you seem to be at the end of this side of the tape. ";
        if (self.current_track == SIDE_START) self.current_track = FIRST_TRACK;
        print"With a satisfying ~click~ the play button engages.^";
        walkman_playing = true;
        ],
    advance [;
        self.current_track++;
        if ((self ~= thriller && self.current_track == SIDE_END) || (self == thriller && self.current_track == THRILLER_SIDE_END))
        {
            walkman_playing = false;
            print"With a ~click~ the tape comes to the end of the side and the play button disengages.^";
            ClearFlag(F_WALKMAN_BLOCKING);
        rtrue;
        }   
    ],
    before [ x ;
        flip:
            walkman_playing = false;
            ClearFlag(F_WALKMAN_BLOCKING);
            if (self.current_side == SIDE_A) self.current_side = SIDE_B; else self.current_side = SIDE_A;
            x = self.current_track;
            give walkman ~open;
            print"You pop out the tape, flip it over, put it back in, and snap the cassette compartment closed.^"; 
            self.tape_advance = 0;
            if (self ~= thriller && x == SIDE_END) { self.current_track = FIRST_TRACK; rtrue; }
            if (self == thriller && x == SIDE_END) { self.current_track = FIRST_TRACK; rtrue; }
            if (x == SIDE_START)
            { 
                if (x ~= thriller)  { self.current_track = SIDE_END; rtrue; }
                if (x == thriller)  { self.current_track = THRILLER_SIDE_END; rtrue; }
            }
            if (self ~= thriller) x = SIDE_END - x;
            if (self == thriller) x = THRILLER_SIDE_END - x;
            self.current_track = x;
            give walkman ~open;
            rtrue;
    ],
    rewind [ i;
        walkman_playing = false;
        ClearFlag(F_WALKMAN_BLOCKING);
        self.tape_advance = 0;
        if (self.current_track == SIDE_START) { self.current_track = FIRST_TRACK; "The tape seems to already be rewound to the beginning. "; }
        i = self.current_track;
        i--;
        self.current_track = i;
        if (self.current_track == SIDE_START) "You press the button down and with a ~whir~ the little spools spin rapidly
            backwards. With a ~click~ you reach the beginning of the side. "; 
            "You press the button down and with a ~whir~ the little spools spin rapidly backwards. After a moment you 
            release the button. ";
    ],
    class Floatable Item;

Tape mixtape "yellow cassette tape"
    with 
        name 'yellow' 'cassette' 'tape' 'mixtape',
        description"It's a mixtape your friend Jennifer lent you. ",
        current_side SIDE_A,
        current_track FIRST_TRACK,
        playback [ ;
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
            if(self in walkman && walkman has worn && walkman_playing && walkman in player && FlagIsClear(F_HEADPHONES_ARE_UNPLUGGED))
             { SetFlag(F_WALKMAN_BLOCKING); self.playback(walkman); }
        ];

Tape thriller "red cassette tape" walkman
    with 
        name 'red' 'tape' 'cassette' 'thriller',
        description"It's a red cassette tape. The label reads ~Thriller~.",
        current_side SIDE_A,
        current_track FIRST_TRACK,
        playback [ ;
            print"^Through the headphones you hear ";
            if (self.current_side == SIDE_A)
                {
                    switch (self.current_track) 
                    {
                    1:  print"~Wanna Be Startin' Somethin'~";
                    2:  print"~Baby Be Mine~";
                    3:  print"~The Girl Is Mine~";
                    4:  print"~Thriller~";
                    5:  print"empty tape";
                    }
                }
                if (self.current_side == SIDE_B)
                {
                    switch (self.current_track) 
                    {
                    1:  print"~Beat It~";
                    2:  print"~Billie Jean~";
                    3:  print"~Human Nature~";
                    4:  print"~P.Y.T. (Pretty Young Thing)~";
                    5:  print"~The Lady in My Life~";
                    }
                }
                ".";
        ],
        each_turn [;
            if(self in walkman && walkman has worn && walkman_playing && walkman in player && FlagIsClear(F_HEADPHONES_ARE_UNPLUGGED))
             { SetFlag(F_WALKMAN_BLOCKING); self.playback(walkman); }
        ];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object rock "rock" aquarium 
    with 
        name 'decorative' 'rock',
        mass 8,
        description "It's a heavy, decorative chunk of rock, about twice as large as one of your fists. ",
        before [;
            take:
            if (self in aquarium)
            {
                give aquarium container;
                give aquarium open;
                move self to player;
                move jorry_tape to aquarium;
                "You steel yourself to reach into the tank. Lifting up the rock, you find a green cassette tape lying under
                it. ";

            } 
            lookunder:
            if (jorry_tape in self)
            {
                give aquarium container;
                give aquarium open;
                move jorry_tape to aquarium;
                "You lift the rock and find a green cassette tape lying under it. ";
            }
        ],
    class Item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Tape jorry_tape "green cassette tape" rock
    with name 'green' 'cassette' 'tape',
        description"It's a green cassette tape. Written on it you see ~S.J.~",
        current_side SIDE_A,
        current_track FIRST_TRACK,
        playback [;
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
        each_turn [;
            if(self in walkman && walkman has worn && walkman_playing && walkman in player && FlagIsClear(F_HEADPHONES_ARE_UNPLUGGED))
             { SetFlag(F_WALKMAN_BLOCKING); self.playback(walkman); }
        ],
        after [;
            take:
            Achieved(13);
        ];

[ jorry_confession ;
    jorry_tape.current_track = 2;
    print"the sounds of a tape-recorder motor and then a voice, distorted and speaking a bit too excitedly
    into the microphone.^^~Hello, my name is Sid Jorry. I am CFO of St. Asterix hospital. I plan to mail this to you 
    because I have information you may find interesting pertaining to possible financial misdealings perpetrated by 
    Dr. Walt Northrup, CEO of the hospital.~^^
    He clears his throat awkwardly and you hear paper rustling.^^
    ~It seems the good doctor has been siphoning money from the hospital with the expectation that it would fail. At 
    the same time, Dr. Northrup has become a shadow investor in the company that owns the hospital building and grounds.~^^
    He pauses and you hear what sounds like drinking from a glass.^^
    ~The hospital is likely worth much more as real estate than as a hospital and its failure would be a windfall for 
    several powerful people. I have records of all of this in a ledger locked in my safe. To delay Northrup from finding it, 
    I have secretly swapped safes with him. My safe is now in his office. The combination is ";
    print SAFE_1,", ",SAFE_2,", ",SAFE_3,".~^^";
    print"You hear a heavy ~thump~ and a cough.^^~If anything happens to me, it's almost certainly not an accident and this 
    information should be given to the proper authorities.~^^Then there's a loud ~click~ and empty hissing. You press 'stop'";
    walkman_playing = false;
    SetFlag(F_COMBO_HAS_BEEN_HEARD);
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Tape air_supply_tape "blue cassette tape" environmental_desk
    with 
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'blue' && w2 == 'tape' or 'cassette') return 2;
            if (w1 == 'air' && w2 == 'supply') return 2;
            if (w1 == 'blue' or 'tape' or 'cassette') return 1;
        ],
        description"It's a blue cassette tape. It's labelled ~Air Supply - Greatest Hits~.",
        current_side SIDE_A,
        current_track FIRST_TRACK,
        playback [;
            print"^Through the headphones you hear ";
                if (self.current_side == SIDE_A)
                {
                    switch (self.current_track) 
                    {
                    1:  print"~Lost In Love~";
                    2:  print"~The One That You Love~";
                    3:  print"~Here I Am~";
                    4:  print"~Sweet Dreams~";
                    5:  print"~Bring out the Magic~";
                    6:  print"~Love and Other Bruises~";
                    }
                }
                if (self.current_side == SIDE_B)
                {
                    switch (self.current_track) 
                    {
                    1:  print"~Chances~";
                    2:  print"~Every Woman in the World~";
                    3:  print"~All Out of Love~";
                    4:  print"~Even the Nights Are Better~";
                    5:  print"~Two Less Lonely People in the World~";
                    6:  print"~Making Love out of Nothing at All~";
                    }
                }
                print".^";
        ],
        each_turn [;
        if(self in walkman && walkman has worn && walkman_playing && walkman in player && FlagIsClear(F_HEADPHONES_ARE_UNPLUGGED))
             { SetFlag(F_WALKMAN_BLOCKING); self.playback(walkman); }
        ];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object shrimp_bowl "bowl" security_desk
    with name 'blue' 'bowl',
        description [;
            print"It's a blue bowl, ";
            if (children(self) == 0)    "currently empty. "; else print"currently holding ", (a)(child(self)),".^";
        ],
        mass 5,
        max_capacity 4,
        describe [;
            if (self in security_desk) rtrue;
        ],
        before [;
            take:
                if (buzz in security_chair) "Buzz, irritated, stops you. ~Can I help you?~";
            receive:
		if (buzz in security_chair && noun == shrimp)
		{
                move shrimp to shrimp_bowl;
                "Buzz's eyes widen at the sight of the shrimp.^
                ~Ooh, thought you could get away, huh?~^
                Before you can react, he plops the shrimp in the bowl with the others. ";
            	}
                if (buzz in security_chair) "Buzz, irritated, stops you. ~Can I help you?~";
        ],
        after [;
                take:
                    give self ~concealed;
        ],
    class Item
    has container open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object cocktail_shrimp "bowlful of shrimp" shrimp_bowl
    with name 'cocktail' 'shrimp',
        description "It's a bowlful of cocktail shrimp. ",
        mass 4,
        before [;
            take:
            if (buzz in security_chair) "Buzz, stops you. 
            ~Excuse me. Would you mind keeping your paws out 
            of my lunch?~"; else "Ew, no. They're slimy and gross. ";
            eat:
            "They don't look very appetizing and your mom insists you once swelled up after your 
            Uncle Tommy gave you lobster once when you were a toddler. ";
        ],
    has static edible;

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
            take:
            if (self in shrimp_bowl) "Buzz smacks your hand away. ~Get your own.~";
        ],
        describe [; if(self in garbage_can) rtrue;],
    class Floatable Item
    has edible;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object band_aid "band-aid" sid_jorry
    with name 'band-aid' 'bandaid' 'bandage',
        description [;
            print"It's a standard band-aid";
            if (self in sid_jorry) ", currently stuck to Sid Jorry's neck. "; else ".";
        ],
        before [;
            take:
            if (self in sid_jorry) 
            { 
                move self to player; 
                move puncture_mark to sid_jorry;
                "You steel yourself and peel the band-aid off. Eww! Underneath you see a tiny puncture mark. ";
            }
            puton:
            "Putting a band-aid back on never works. ";
        ],
        mass 0,
    class Floatable Item;

Object puncture_mark "puncture mark" 
    with name 'puncture' 'wound' 'mark',
        description "It's a tiny puncture wound in his neck. ",
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object denim_jacket "denim jacket" boiler_room
    with name 'denim' 'jacket',
        mass 12,
        add_to_scope jacket_pocket,
        description "It's a large men's denim jacket with a single pocket. ",
        initial "There's a denim jacket here balled up on the ground. ",
        before [;
            search:
                <<search jacket_pocket>>;
        ],
    class Item
    has clothing;

MyContainer jacket_pocket "jacket pocket"
    with 
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'jacket' && w2 == 'pocket') return 2;
            if (w1 == 'pocket' && w2 == 0)  return 1;
        ],
        max_capacity 4,
        before [;
            examine:
            <<search self>>;
        ],
        has container open scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object syringe "syringe" jacket_pocket
    with name 'syringe' 'needle',
        description"It's an empty syringe with a needle and cap on it. ",
        mass 1,
    before [;
        take:
            if (self in jacket_pocket) move self to real_location;
            ActivateTopic(vic, 300);
            ActivateTopic(nurse_retch, 300);
            Achieved(2);
    ],
    class Floatable Item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object ladder "ladder" environmental_services
    with name 'ladder' 'metal' 'folding',
    mass 25,
        description [;
            print"It's a six-foot aluminum folding ladder, it's currently ";
            if (self has open) "unfolded. "; "folded up. ";
        ],
        before [;
            climb, enter:
            if (parent(self) == player) "You can't climb it if you're carrying it. ";
            if (self hasnt open) "You need to unfold the ladder first. ";
            if (real_location == hallway_m2)
            {
                print"You climb the creaky metal ladder nearly to the top. Once there, you are able to 
                grab onto some sturdy pipes that you see through the gap in the tiling and pull yourself into the ceiling above. 
                Breathing heavily but victorious, you think back to P.E. class freshman year when you couldn't climb a knotted 
                rope in gym class and Becky Harris laughed at you. Eat it Becky!^^";
                PlayerTo(ceiling_01);
                rtrue;
            } 
            PlayerTo(self, 1);
            "You climb up onto the aluminum ladder. ";
        ],
        react_before [;
            go:
            if(selected_direction == u_to && player in self)
            {
                if (real_location == hallway_m2)
                {
                    print"You climb the creaky metal ladder nearly to the top. Once there, you are able to 
                    grab onto some sturdy pipes that you see through the gap in the tiling and pull yourself into the ceiling above. 
                    Breathing heavily but victorious, you think back to P.E. class freshman year when you couldn't climb a knotted 
                    rope in gym class and Becky Harris laughed at you. Eat it Becky!^^";
                    PlayerTo(ceiling_01);
                    rtrue;
                } 
                "There's nowhere to go upward. ";
            } 
        ],
    class Item
    has openable supporter enterable;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object kcl_bottle "plastic vial"
    with 
        name 'kcl' 'potassium' 'bottle' 'vial',
        description "It's a small plastic bottle labelled ~Potassium Chloride~. The seal 
        on top has been broken and the vial is half-empty. ",
        after [;
            take:
            ActivateTopic(oliver, 303);
            ActivateTopic(nurse_retch, 301);
            Achieved(3);
        ],
        mass 2,
    class Item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object ledger "ledger" northrup_safe
    with 
        name 'ledger' 'leather' 'book',
        description "It's a leather-bound book, wider than it is tall. It's filled with lined pages
        separated into columns and full of numbers written in fine handwritten ink. The whole think reeks
        of math and totally bores you. ",
        before [;
            open, read:
            "You flip through the pages of numbers and are immediately reminded of your algebra class freshman year and
            that weird guy you sat next to who would only shower, like, twice a week?";
        ],
        after [;
            take:
            Achieved(15);
        ],
        mass 10,
    class Item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Array signatures --> 17;
Object form "signature form" 
    with 
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'signature' && w2 == 'form') return 2;
            if (w1 == 'form') return 1;
        ],
        description [ x ;
            print"It's a signature form on your high school's letterhead. Below the text there are eight lines 
            for staff or patient signatures. Currently, there ";
            x = SIGNATURE_COUNT;
            if (x == 0) "are no signatures. ";
            if (x == 1) { print"is one signature: "; }
            else { print"are ",x," signatures: "; }
            show_signatures();
            ".";
        ],
        before [ ;
            read:
            <<examine self>>;    

            sign:
            "Are you forging signatures on a form for your Ethics class? ";        
        ],
        mass 2,
    class Item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object letter "piece of paper"
    with 
        name 'letter' 'paper',
        description "It's a letter signed by Sid Jorry confirming your successful participation in 
        and completion of your volunteer experience at St. Asterix. ",
        mass 1,
        describe [;
            if (self in mri_scanner && self.fluttering == 1) { self.fluttering = 0; rtrue; }
        ],
        fluttering,
        before [;
            take:
            Achieved(17);
            examine:
            if (self notin player) "You can't make out what's on the paper from here. ";
        ],
        after [;
            take:
            print"^You take the letter and see, to your joy and excitement, that it is indeed your signed letter of completion. 
            You feel like you could kiss Mr. Jorry, if he weren't already dead.^^ 
            Just then, to your relief, several shivering police officers dusted with snow enter the room. The one in charge stops cold 
            at the sight in front of her but you quickly bring her up to speed and, armed with the evidence you've collected, 
            the mangled trio on the floor are read their rights and hustled away.^";

            if (SIGNATURE_COUNT > 7)
            {
                print"^And having obtained all your necessary signatures, you happily end your time as a candy striper and 
                go on to have your best summer ever at the pool.^";
                deadflag = 2;
                rtrue;
            }
            print"^Unfortunately, you did not obtain all the necessary signatures on your signature form. You would think that, 
                considering the circumstances there would be some leniency. But Mr. Rogers is a hard man and it IS an ethics class. 
                Unfortunately, your summer is ruined as you are stuck in summer school.^";
                deadflag = 3;
                rtrue;
        ],
    class Item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object switchblade "switchblade knife"
    with 
        name 'knife' 'switchblade',
        description "It's a nasty knife. ",
        mass 1;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object pen "ballpoint pen" 
    with 
        name 'pen' 'ballpoint',
        description "It's a standard ballpoint pen. ",
        mass 0,
    class Floatable Item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object dull_key "dull steel key" freddy !unlocks jorry's office
    with 
        name 'dull' 'steel' 'key',
        description "It's a standard steel key. ",
        mass 0,
        after[;
            take:
            Achieved(4);
        ],
    class item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
OnChair folding_chair "folding chair" boiler_room
    with 
        name 'folding' 'chair',
        description "It's a standard metal folding chair. ",
        mass 15,
    class Item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object ashtray "ashtray" boiler_room
    with 
        id, ! 0 = ashtray, 1 = butts and ashes 
        parse_name [ w1 ;
            w1 = NextWord();
            if (w1 == 'ashtray') { self.id = 0; return 1; }
            if (w1 == 'cigarettes' or 'cigarette' or 'butt' or 'butts' or 'ashes') { self.id = 1; return 1; }
        ],
        article "an",
        description [;
            if (self.id == 0 ) "It's a dirty ashtray. ";
                "It's a collection of ashes and half-smoked and crushed out cigarette butts. ";
        ],
        before [;
            take:
            "You reach for the ashtray then recoil in disgust.  Just ~Yuck!~ ";
        ],
        mass 3,
    class Item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object backpack "backpack" 
    with 
        name 'backpack' 'bag' 'sack' 'pack',
        description "It's your neon teal Jansport backpack. ",
        before [;
            receive:
            if (noun == ladder or folding_chair) "That's too big to fit. ";
            if (noun == shrimp_bowl) "Ew! And get gross slimy shrimp all over your backpack? No way. ";
            if ((self.is_holding() + noun.mass) > 20 ) "That won't fit in your backpack. ";
        ],
        is_holding [i total;
            total = 0;
            objectloop(i in self)
            {
                total = total + i.mass;
            }
            return total;
        ],
        mass 10,
    class Item
    has container openable ~open clothing;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object plastic "round piece of plastic" engineering
    with 
        name 'round' 'piece' 'plastic',
        description "You can't really make out any details while it's inside the fuse box. ",
        mass 1,
        noticed 0,
        describe [; if (self hasnt moved) rtrue; ],
        before [;
            take:
            if (self.noticed == false) "You can't see any such thing. ";
        ],
        after [;
            take, pull:
            if (self hasnt moved) 
            {
                print"There's a tiny spark across the unblocked contact and you hear a deep electrical hum start somewhere in the 
                bowels of the building around you. After a moment, the hum subsides.^";
                SetFlag(F_SERVICE_ELEVATOR_ON);
                remove self;
                move coaster to player;
                scope_modified = true;
                ActivateTopic(nurse_retch, 306);
                ActivateTopic(vic, 301);
                Achieved(16);
                rtrue;
            }
        ], 
    class Item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object coaster "coaster"
    with
        name 'coaster' 'round' 'piece' 'plastic',
        description "It's a plastic coaster from 'Mama Leone's Ristorante and Legitimate 
        Businessmen's Club'. Their catchphrase below reads: ~We Ain't Seen Nothin'.~",
        mass 1,
    class Item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object balloon "helium balloon" room_22
    with 
        id, ! 0 = balloon, 1 = string
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'helium' && w2 == 'balloon') { self.id = 0; return 2; }
            if (w1 == 'mylar' && w2 == 'balloon') { self.id = 0; return 2; }
            if (w1 == 'balloon' or 'helium' or 'mylar') { self.id = 0; return 1; }
            if (w1 == 'string') { self.id = 1; return 1; }
        ],
        tied_to 0,
        invent [ ;
        if (inventory_stage == 2)
        {
            if(self.tied_to ~= 0) print" (which is tied to ",(the)self.tied_to,")"; rtrue;
        }
    ],
        mass 0,
        description [;
            if(self.id == 0)
            {
                print"It's a silver mylar helium balloon. On the side it reads ~Get Well Soon!~ in large cartoonish letters. ";
                if (self.tied_to == 0) "It's floating here, bumping against the ceiling. ";
                if (self.tied_to == player) "It's floating here, held by the string. ";
                "It's currently tied to ",(a)self.tied_to,".";
            }
            "The string hangs down from the balloon. ";
            ],
        before [;
            tie:
            if (self.tied_to == second) "It's already tied to ",(the)second,".";
            if (~~second ofclass Item) "There's no good place to tie onto ",(the)second,".";
            if (self.tied_to ~= 0 && self.tied_to ~= player) "You would first need to untie it from ",(the)self.tied_to,".";
            if (self.tied_to == 0 && self.tied_to ~= player) { print"(first taking the balloon)^"; move self to player; }
            self.tied_to = second;
            if (self.tied_to has worn) give self.tied_to ~worn;
            move second to self;
            "You tie the balloon to ",(the)second,".";

            insert:
            if (self.tied_to == 0 && second ofclass DumbwaiterShaft)
            {
                move balloon to second_floor_dumbwaiter;
                if (real_location == hallway_2_2) "You put ",(the)self," in the dumbwaiter. It floats gently upward suspended 
                from the balloon and bounces on the roof of the shaft here. ";
                "You put ",(the)self," in the dumbwaiter. It rises up and out of sight. ";
            }
            if (self.tied_to ~= 0 && second ofclass DumbwaiterShaft)
            {
                <<insert self.tied_to second>>;
            }

            untie:
            if (self.tied_to == 0) "It's not tied to anything. ";
            print"You untie the balloon from ",(the)self.tied_to,".^";  
            move self.tied_to to parent(balloon);
            self.tied_to = 0;
            rtrue;                  

            take:
            if (self.tied_to == 0) 
            {
                move self to player;
                scope_modified = true;
                "Taken. ";
            }
            move self to player;
            scope_modified = true;
            "Taken.";
        ],
        after [;
            take:
            self.tied_to = player;
        ],
    class Item
    has transparent container open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object darkroom_flashlight "flashlight" darkroom_counter
    with 
        name 'light' 'flashlight' 'torch',
        description [;
            print"It's a flashlight with a special bulb installed for use in darkrooms. It's ";
            if(self has on) "currently on and emitting a creepy red light. "; "currently off. ";
        ],
        mass 5,
        after [;
            switchon:
                give self light;
                print"^";
            switchoff:
                give self ~light;
                print"^";
	    take:
		Achieved(0);
        ],
    class Item
    has switchable;

