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
                service_elevator_call_level = -1;
                StartDaemon(service_elevator_daemon);
                service_elevator_active = true;
                give card_reader on;
                StopTimer(service_close_door_timer);
                if (service_elevator_door has open) print"The elevator door slides closed. ";
                "The little light on the card reader turns green and you feel the elevator moving beneath you. ";
            }
            swipethrough:
            if (second == card_reader) 
            {
                service_elevator_call_level = -1;
                StartDaemon(service_elevator_daemon);
                service_elevator_active = true;
                give card_reader on;
                StopTimer(service_close_door_timer);
                if (service_elevator_door has open) print"The elevator door slides closed. ";
                "The little light on the card reader turns green and you feel the elevator moving beneath you. ";
            }
        ],

        has item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object flashlight "flashlight" mabel
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
Object dummy_headphones "dummy headphones" 
    with
        name 'dummy' 'headphones' 'phones',
        article "a pair of",
        description "You see two poofy orange ear pieces at the end of a round strip of metal. ",
        before [ t_obj ;
            plugin:
            if (dummy_headphones in player)
            {
                print"(into the walkman)^";
            }
            move walkman to parent(dummy_walkman);
            t_obj = dummy_walkman.tape_is_loaded();
            if (t_obj ~= 0) move t_obj to walkman;
            if (self has worn) give walkman worn;
            walkman_playing = dummy_walkman_playing;
            remove dummy_headphones;
            remove dummy_walkman;
            "You plug the headphones into the walkman. ";
        ]
    has item clothing;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object dummy_walkman "dummy walkman" 
    with 
        name 'walkman' 'player',
        description [ obj;
            print"It's a portable tape player. The tape compartment in the side is 
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
            if(dummy_walkman_playing == true) "and that the little rotors are turning. "; "and that the little 
            rotors are motionless. ";
        ],
        before [;
            open:
                <<push wm_eject_button>>;
            receive:
            if (children(self) > 5 ) "There's already a tape in the walkman. ";
            wear:
            if(self has worn) "You're already wearing the headphones. ";
            give self worn;
            "The headphones aren't plugged in. ";
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
        each_turn [ obj;
            obj = dummy_walkman.tape_is_loaded();
                if(obj && dummy_walkman_playing == true)
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
    has container transparent openable item clothing;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object walkman "walkman" station_b
    with 
        parse_name [ w1;
            w1 = NextWord();
            if (w1 == 'walkman' or 'player') { self.id = 0; return 1; }
            if (w1 == 'headphones' or 'phones') { self.id = 1; return 1; }
        ],
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
        ],
        before [ t_obj ;
            open:
                <<push wm_eject_button>>;
            receive:
            if (children(self) > 5 ) "There's already a tape in the walkman. ";
            wear:
            if(self has worn) "You're already wearing the headphones. ";
            give self worn;
            "You put the headphones on. ";
            unplug:
            t_obj = walkman.tape_is_loaded();
            if (t_obj ~= 0) move t_obj to dummy_walkman;
            if (walkman has worn) 
            {
                move dummy_headphones to parent(walkman);
                give dummy_headphones worn;
            }
            move dummy_walkman to parent(walkman);
            move dummy_headphones to parent(walkman);
            dummy_walkman_playing = walkman_playing;
            remove walkman;
            "You pull the headphone plug out of the audio jack. ";
            plugin, pluginto:
            "The headphones are already plugged into the walkman. ";
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
    has container transparent openable item clothing;

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
        ],
        article "an",
        description"It's a chunky black button with the 'eject' symbol on the top. ",
        before [;
            push:
                if (walkman has open) "The walkman is already open. ";
                walkman_playing = false;
                give walkman open;
                "You press the 'eject' button and the tape compartment springs open with a ~clatter~.";
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
        ],
        description"It's a chunky black button with the 'stop' square on the top. ",
        before [;
            push:
                if(~~walkman_playing) "It's already stopped. ";
                walkman_playing = false;
                "You press the button and the 'play' button disengages with a ~clunk~. ";
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
        ],
        description"It's a chunky black button with two 'FF' arrows on the top. ",
        before [ obj;
            push:
                obj = walkman.tape_is_loaded();
                if(obj)
                {
                    obj.fast_forward(walkman);
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
        ],
        description"It's a chunky black button with two backwards 'rewind' arrows on the top.",
        before [ obj;
            push:
                obj = walkman.tape_is_loaded();
                if(obj)
                {
                    obj.rewind(walkman);
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
Global boombox_playing = true;
Object boombox "boombox" admin_hallway
    with 
        name 'boombox' 'box' 'player' 'compartment',
        describe [;
            if (self hasnt moved) "A boombox sits on the floor next to Freddy. ";
        ],
        description [ obj;
            print"It's a boxy silver plastic and metal boombox with a black carrying handle. The tape compartment in 
            the side is currently ";
            if(self has open) print"open. "; else print "closed. ";
            print"Several chunky black buttons protrude from the top including 'play', 'stop', 'fast fwd', 'rewind', 'eject'. ";
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
            if(boombox_playing) "and that the little rotors are turning. "; else "and that the little 
            rotors are motionless. ";
        ],
        
        before [;
            open:
                <<push bb_eject_button>>;
            take:
            if (self in admin_hallway && freddy in folding_chair && FlagIsClear(F_FREDDY_ASLEEP)) 
                "Freddy stirs himself. ~Hey, hands off my tunes, man.~";
            receive:
            if (children(self) > 5 ) "There's already a tape in the boombox. ";
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
        mass 6, 
        each_turn [ obj;
            obj = boombox.tape_is_loaded();
                if(obj && boombox_playing == true)
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
    has container transparent openable item;

Object bb_eject_button "boombox eject button" boombox
    with 
        parse_name [ w1 w2 w3;
            w1 = NextWord();
            w2 = NextWord();
            w3 = NextWord();
            if (w1 == 'boombox' && w2 == 'eject' && w3 == 'button') return 3;
            if (w1 == 'boombox' && w2 == 'eject') return 2;
            if (w1 == 'eject' && w2 == 'button') return 2;
            if (w1 == 'eject') return 1;
        ],
        article "an",
        description"It's a chunky black button with the 'eject' symbol on the top. ",
        before [;
            push:
                if (boombox has open) "The boombox is already open. ";
                boombox_playing = false;
                give boombox open;
                "You press the 'eject' button and the tape compartment springs open with a ~clatter~.";
        ],
    has scenery;

Object bb_play_button "boombox play button" boombox
    with
        parse_name [ w1 w2 w3;
            w1 = NextWord();
            w2 = NextWord();
            w3 = NextWord();
            if (w1 == 'boombox' && w2 == 'play' && w3 == 'button') return 3;
            if (w1 == 'boombox' && w2 == 'play') return 2;
            if (w1 == 'play' && w2 == 'button') return 2;
            if (w1 == 'play') return 1;
        ],
        description"It's a chunky black button with a 'play' arrow on the top. ",
        before [ obj;
            push:
                if(boombox has open) "You should close the tape compartment first. ";
                if(boombox_playing) "The boombox is already playing. ";
                obj = boombox.tape_is_loaded();
                if(obj)
                {
                    obj.press_play(boombox);
                    rtrue;
                } 
            boombox_playing = true;
            "With a satisfying ~click~ the play button engages.";
        ],
    has scenery;   

Object bb_stop_button "boombox stop button" boombox
    with 
        parse_name [ w1 w2 w3;
            w1 = NextWord();
            w2 = NextWord();
            w3 = NextWord();
            if (w1 == 'boombox' && w2 == 'stop' && w3 == 'button') return 3;
            if (w1 == 'boombox' && w2 == 'stop') return 2;
            if (w1 == 'stop' && w2 == 'button') return 2;
            if (w1 == 'stop') return 1;
        ],
        description"It's a chunky black button with the 'stop' square on the top. ",
        before [;
            push:
                if(~~boombox_playing) "It's already stopped. ";
                boombox_playing = false;
                "You press the button and the 'play' button disengages with a ~clunk~. ";
        ],
    has scenery;   

Object bb_fast_forward_button "boombox fast-forward button" boombox
    with 
        parse_name [ w1 w2 w3;
            w1 = NextWord();
            w2 = NextWord();
            w3 = NextWord();
            if (w1 == 'boombox' && w2 == 'fast-forward' or 'fast' && w3 == 'button') return 3;
            if (w1 == 'boombox' && w2 == 'fast' or 'forward') return 2;
            if (w1 == 'fast' && w2 == 'forward') return 2;
            if (w1 == 'fast' or 'forward' && w2 == 'button') return 2;
            if (w1 == 'fast' or 'forward') return 1;
        ],
        description"It's a chunky black button with two 'FF' arrows on the top. ",
        before [ obj;
            push:
                obj = boombox.tape_is_loaded();
                if(obj)
                {
                    obj.fast_forward(boombox);
                } 
                else
                {
                    boombox_playing = false;
                    "You press the button down and with a ~whir~ the little spools spin rapidly. After a moment you 
                    release the button.";
                }
                rtrue;    
        ],
    has scenery;  

Object bb_rewind_button "boombox rewind button" boombox
    with
        parse_name [ w1 w2 w3;
            w1 = NextWord();
            w2 = NextWord();
            w3 = NextWord();
            if (w1 == 'boombox' && w2 == 'rewind' && w3 == 'button') return 3;
            if (w1 == 'boombox' && w2 == 'rewind') return 2;
            if (w1 == 'rewind' && w2 == 'button') return 2;
            if (w1 == 'rewind') return 1;
        ],
        description"It's a chunky black button with two backwards 'rewind' arrows on the top.",
        before [ obj;
            push:
                obj = boombox.tape_is_loaded();
                if(obj)
                {
                    obj.rewind(boombox);
                } 
                else
                {
                    boombox_playing = false;
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
Constant ADVANCE_TIMER = 4;

Class Tape 
    with
        is_in_player [;
        if(self in boombox) return boombox;
        if(self in walkman) return walkman;
        if(self in dummy_walkman) return dummy_walkman;
        rfalse;
    ],
    mass 2,
    current_side,
    current_track,
    tape_advance 0,
    fast_forward [ p i;
        if(p == boombox) boombox_playing = false;
        if(p == walkman) walkman_playing = false;
        if(p == dummy_walkman) dummy_walkman_playing = false;
        self.tape_advance = 0;
        if (self.current_track == SIDE_END) "The tape seems to be at the end of the side. ";
        i = self.current_track;
        i++;
        self.current_track = i;
        !print"in FF new track = ",i,"^";
        if (self.current_track == SIDE_END) "You press the button down and with a ~whir~ the little spools spin rapidly.
            With a ~click~ you reach the end of the side. "; 
        "You press the button down and with a ~whir~ the little spools spin rapidly. After a moment you 
        release the button. ";
    ],
    press_play [ p;
        if (self.current_track == SIDE_END) "You press the button but you seem to be at the end of this side of the tape. ";
        if (self.current_track == SIDE_START) self.current_track = FIRST_TRACK;
        print"With a satisfying ~click~ the play button engages.^";
        if(p == boombox) boombox_playing = true;
        if(p == walkman) walkman_playing = true;
        if(p == dummy_walkman) dummy_walkman_playing = true;
        ],
    !play [;
    !    if (self.current_track == SIDE_END) "The tape seems to be at the end of the side. ";
    !    if (self.current_track == SIDE_START) self.current_track = FIRST_TRACK;
    !    self.playback();
    !],
    advance [;
        self.current_track++;
        if (self.current_track == SIDE_END) 
        {
            if(self in boombox) boombox_playing = false;
            if(self in walkman) walkman_playing = false;
            if(self in dummy_walkman) dummy_walkman_playing = false;
            "With a ~click~ the tape comes to the end of the side and the play button disengages. ";
        }   
    ],
    before [ x;
        flip:
            !print"flipping ",(name)self,"^";
            if(self in boombox) boombox_playing = false;
            if(self in walkman) walkman_playing = false;
            if(self in dummy_walkman) dummy_walkman_playing = false;
            if (self.current_side == SIDE_A) self.current_side = SIDE_B; else self.current_side = SIDE_A;
            !print"now side = ",self.current_side,"^";
            x = self.current_track;
            !print"current track = ",x,"^";
            if(self in boombox) give boombox open;
            if(self in walkman) give walkman open;
            if(self in dummy_walkman) give dummy_walkman open;
            print"You pop out the tape, flip it over, put it back in, and snap the cassette compartment closed.^"; 
            self.tape_advance = 0;
            if (x == SIDE_END) { self.current_track = FIRST_TRACK; rtrue; }
            if (x == SIDE_START) { self.current_track = SIDE_END; rtrue; }
            x = SIDE_END - x;
            self.current_track = x;
            rtrue;
            !print_ret"now current track = ",self.current_track,".";
        ],
    rewind [ p i;
        if(p == boombox) boombox_playing = false;
        if(p == walkman) walkman_playing = false;
        if(p == dummy_walkman) dummy_walkman_playing = false;
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
    has item;

Tape mixtape "casette tape with a yellow label" walkman
    with name 'mix' 'mixtape' 'tape' 'cassette' 'yellow',
        description"It's a cassette tape with a yellow label. You see ~mixtape~ handwritten on it.",
        current_side SIDE_A,
        current_track FIRST_TRACK,
        playback [;
            print"^From the boombox you hear ";
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
            if(self in boombox && boombox_playing && TestScope(self)) self.playback();
        ],
        has item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object rock "rock" aquarium 
    with 
        name 'decorative' 'rock',
        description "It's a decorative chunk of rock, about twice as large as one of your fists. ",
        before [;
            take:
            if (self in aquarium)
            {
                give aquarium container;
                give aquarium open;
                move self to player;
                move jorry_tape to aquarium;
                "You steel yourself to reach into the tank. Lifting up the rock, you find a casette tape lying under
                it. ";

            } 
            lookunder:
            if (jorry_tape in self)
            {
                give aquarium container;
                give aquarium open;
                move jorry_tape to aquarium;
                "You lift the rock and find a casette tape under it. ";
            }
        ],
        mass 5,
    has item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Tape jorry_tape "cassette tape with a green label" rock
    with name 'tape' 'cassette' 'green',
        description"It's a cassette tape with a greenlabel. Written on the label you see ~S.J.~",
        current_side SIDE_A,
        current_track FIRST_TRACK,
        playback [;
            print"^From the boombox you hear ";
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
        if(self in boombox && boombox_playing && TestScope(self)) self.playback();
        ],
    has item;

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
    print SAFE_1,", ",SAFE_2,", ",SAFE_3,".^^";
    print"You hear a heavy ~thump~ and a cough.^^~If anything happens to me, it's almost certainly not an accident and this 
    information should be given to the proper authorities.~^^Then there's a loud ~click~ and empty hissing. You press 'stop'";
    boombox_playing = false;
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Tape air_supply_tape "cassette tape with a blue label" 
    with name 'air' 'supply' 'tape' 'cassette' 'blue',
        description"It's a cassette tape with a blue label. It's labelled ~Air Supply - Greatest Hits~",
        current_side SIDE_A,
        current_track FIRST_TRACK,
        playback [;
            print"^From the boombox you hear ";
                if (self.current_side == SIDE_A)
                {
                    switch (self.current_track) 
                    {
                    1:  print"~Love and Other Bruises~";
                    2:  print"~Bring out the Magic~";
                    3:  print"~The One That You Love~";
                    4:  print"~Here I Am~";
                    5:  print"~Sweet Dreams~";
                    6:  print"~Lost in Love~";
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
                if (FlagIsClear(F_FREDDY_ASLEEP))
                {
                    print"^The tuneful adult contemporary strains of 'Air Supply' wash over the already-significantly 
                    drowsy security guard. Freddy's head bobs once or twice and then his chin hits his chest and he 
                    begins to snore.^";
                    SetFlag(F_FREDDY_ASLEEP);
                }
        ],
        each_turn [;
            if(self in boombox && boombox_playing && TestScope(self)) self.playback();
        ],
        has item;

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
Object shrimp "rotten shrimp" !garbage_can
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
    has item;

Object puncture_mark "puncture mark" 
    with name 'puncture' 'wound' 'mark',
        description "It's a tiny puncture wound in his neck. ",
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object denim_jacket "denim jacket" boiler_room
    with name 'denim' 'jacket',
        mass 12,
        description "It's a large men's denim jacket with a single pocket. ",
        before [;
            search:
                <<search jacket_pocket>>;
        ],
    has clothing item transparent;

MyContainer jacket_pocket "jacket pocket" denim_jacket
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
        has open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object syringe "syringe" jacket_pocket
    with name 'syringe' 'needle',
        description"It's an empty syringe with a needle and cap on it. ",
        mass 1,
    before [;
        take:
            if (self in jacket_pocket) move self to real_location;
    ],
    has item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object ladder "ladder" engineering
    with name 'ladder' 'metal' 'folding',
    mass 26,
        description [;
            print"It's a six-foot aluminum folding ladder, it's currently ";
            if (self has open) "unfolded. "; "collapsed. ";
        ],
        before [;
            climb, enter:
            if (parent(self) == player) "You can't climb it if you're carrying it. ";
            if (self hasnt open) "You need to unfold the ladder first. ";
            PlayerTo(self, 1);
            "You climb up onto the aluminum ladder. ";
        ]
    has openable supporter enterable item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object kcl_bottle "plastic vial" nurse_retch
    with 
        name 'kcl' 'potassium' 'plastic' 'bottle' 'vial',
        description "It's a small plastic bottle labelled ~Potassium Chloride~. The seal 
        on top has been broken and the vial is half-empty. ",
    has item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object ledger "ledger" northrup_safe
    with 
        name 'ledger' 'leather' 'book',
        description "It's a leather-bound book, wider than it is tall. It's filled with lined pages
        separated into columns and full of numbers written in fine handwritten ink. The whole think reeks
        of math and totally bores you. ",
        before [;
            open:
            "You flip through the pages of numbers and are immediately reminded of your algebra class freshman year and
            that weird guy you sat next to who would only shower, like, twice a week?";
        ],
    has item;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object letter "letter" 
    with 
        name 'letter' 'confirmation',
        description "It's a typed letter signed by and on Sid Jorry's letterhead. ",
        initial "A piece of paper is here fluttering in the air. ",
        before [;
            read:
            "~This will confirm that Candy Striper has successfully completed her mandatory volunteer
            assignment here at St. Asterix.^^Sincerely,^Sid Jorry, CFO";
        ],
        after [;
            take:
            print"YOU WIN^^";
            deadflag = 2;
            rtrue;
        ],
    has item;