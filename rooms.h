Class InChair
    with 
        before [ obj;
            take:
                objectloop(obj in self && obj has animate && obj ~= player) "The chair is currently occupied."; 
            enter:
                if (self == parent(player)) "But you're already sitting in it. ";
                objectloop(obj in self && obj has animate && obj ~= player) "The chair is currently occupied by ", (the) obj, "."; 
                if (child(self) ~= nothing) "The chair is already occupied.";  
                move player to self;
                if (verb_word == 'stand') "You stand on ", (the) self, "."; else "You settle into ", (the) self, ".";
        ],
    has supporter enterable;

Class OnChair
    with 
        before [ obj;
            take:
                objectloop(obj in self && obj has animate && obj ~= player) "The chair is currently occupied."; 
            enter:
                if (self == parent(player)) "But you're already sitting on it. ";
                objectloop(obj in self && obj has animate && obj ~= player) "The chair is currently occupied by ", (the) obj, "."; 
                if (child(self) ~= nothing) "The chair is already occupied.";  
                move player to self;
                if (verb_word == 'stand') "You stand on ", (the) self, "."; else "You sit down on ", (the) self, ".";
        ],
        after [ obj;
            examine:
                objectloop(obj in self && obj has animate) print (The)obj, " is currently sitting in it.^";
        ], 
    has supporter enterable;

Class Bed
    with sit_or_lie,
        before [ obj;
            enter:
                objectloop(obj in self && obj has animate && obj ~= player) "There's already someone in the bed."; 
                if (verb_word == 'sit' or 'lie' or 'get')    {
                    if (self == parent(player) && self.sit_or_lie == verb_word)
                    "You're already on it. ";
                    move player to self;
                    self.sit_or_lie = verb_word;
                    if (AfterRoutines() || keep_silent) rtrue;
                    if (verb_word == 'sit')
                        "You sit on ", (the) self, ".";
                    else    
                        "You lie on ", (the) self, ".";
                }   else    {
                    if (self == parent(player) && self.sit_or_lie == false)
                        "You're already on it. ";
                        move player to self;
                        self.sit_or_lie = false;
                        if (AfterRoutines() || keep_silent) rtrue;
                        "You stand on ", (the) self, ".";
                }

            take:
                "It's quite heavy and you would likely tear something internally. ";
        ],
    has supporter enterable scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room morgue "Morgue"
    with name 'morgue',
        description [;
                print "This is the hospital's small morgue. The white tiled floor is scuffed and gently slopes toward a drain in the center of the
                room. The walls are also clad in featureless tiling and a pair of flickering fluorescent tubes above you impart a sickly green 
                ambiance. A stainless steel table sits in the center of the room and one wall is occupied by a bank of refrigerated
                lockers. A door, currently ";
                open_or_closed(morgue_door);
                " lies to the west. ";
            ],
        cheap_scenery  
            3 'white' 'tile' 'tiles//p' "The tiles are cracked and there's some greenish gunk growing in the spaces between them. "
            1 'drain' "It's a standard drain in the floor about three inches across. "
            3 'flickering' 'fluorescent' 'tubes//p' "They're standard fluorescent lights. They emit a sickly green light that 
                makes everyone look corpse-like. "
            3 'refrigerated' 'locker' 'lockers//p' [;
				examine: "You see a bank of six metal doors with metal handles. ";
                open: "You hesitantly open one a crack and peek inside. Luckily, it's empty. ";
				Take, Remove, Turn, Pull, Push: "You decide you don't really need morgue refrigerators. ";
			],
        w_to morgue_door,
    has light;

Object morgue_table "metal table" morgue
    with name 'metal' 'table',
        description "It's a narrow stainless steel table that stands about waist level. ",
    has scenery supporter;

Object body "body" morgue_table
    with name 'sid' 'jorry' 'body' 'corpse',
        description "It's the former Sid Jorry, the hospital CFO. He reminds you of your Uncle Sid, a corpulent fellow 
        with a large bushy mustache and a ring of hair encircling his otherwise bald head. His eyes are closed (thank goodness) 
        and his complexion is sallow. There's an IV placed in the back of his left hand. ",
        before [;
            take:
            "He'd just be dead weight. ";
        ],
    has scenery;

 !%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor morgue_door "morgue door" 
    with name 'door',
        description [;
            print"It's an unremarkable door, currently ";
            open_or_closed(self);
            ".";
        ],
        npc_open [ npc;
            if (TestScope(npc, player) && self hasnt open) print(The)npc," pushes open the door.^";
            give self open;
            give self ~locked;
        ],
        door_to [;
            if (parent(self) == morgue) return basement_hallway_east; return morgue;
        ],
        door_dir [;
            if (parent(self) == morgue) return w_to; return e_to;
        ],
        found_in morgue basement_hallway_east,
    has scenery door openable ~open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room basement_hallway_east "Basement Hallway"
    with description "This is the eastern end of a long sterile hallway that continues far to the west. Linoleum tiles cover the floor. A door
                    leads east, a placard next to it reads ~Morgue~.",
        cheap_scenery  
            'sign' 'placard' "It's a sign posted on the wall next to the door. It reads ~Morgue~.",
        e_to morgue_door,
        w_to elevator_lobby_b,
    has light; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room elevator_lobby_b "elevator_lobby_b" 
    with description [; 
        print"This is a spartan fluorescent-lit hallway that continues to the east and west.  A stairwell lies to the north through an 
        open doorway. Opposite the stairwall in the south wall stands the main elevator. Next to it you see a sign reading ~Morgue~ above an arrow 
        pointing east and one reading ~Central Supply~ above one pointing west. The elevator doors are currently ";
        open_or_closed(elevator_doors);
        ". The call button is embedded in a small panel next to the door.";
        ],
        e_to basement_hallway_east,
        n_to stairwell_b,
        s_to elevator_doors,
        in_to elevator_doors,
        w_to basement_hallway_west,
    has light,
    ;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room stairwell_b "stairwell_b" 
    with description
        "This is a dark stairwell just off the main hallway which lies to the south. A large ~B~ is painted on the wall. 
        Concrete stairs zig-zag upward to the floor above.",
        s_to elevator_lobby_b,
        u_to stairwell_m,
    has light stairs,
    ;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room basement_hallway_west "basement_hallway_west"
    with description
        "This is nearly the midpoint of a long dank east-west hallway. A service elevator is here to the north. A sign next to it 
        reads ~Employees Only~.",
        e_to elevator_lobby_b,
        w_to hallway_b1,
        n_to service_elevator_door,
        in_to service_elevator_door,
        has light,
        ;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_b1 "hallway_b1"
    with description "The corridor here continues east and west. Embedded here in the wall to the south you see an 
    old dumbwaiter, presumably once used to deliver needed items to the patient floors above.",
    e_to basement_hallway_west,
    w_to hallway_b2,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_b2 "hallway_b2"
    with description "This is the western end of a long corridor that stretches to the east under flickering fluorescent lights. 
        There's an open doorway to the north, a sign next to it reads ~Environmental Services~.
        A second doorway lies to the west. A sign next to it reads ~Engineering~. Finally, a substantial metal door lies to the 
        south, a placard on it reads ~Central Supply~.",
        e_to hallway_b1,
        n_to environmental_services,
        s_to central_supply_door,
        w_to engineering,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room central_supply "Central Supply" 
    with description "This is a large basement filled with dusty crates and defunct antique equipment. There are scattered low-wattage lamps 
    hanging from the ceiling but most of these have long ago burned out, leaving the room quite dark with grotesque shadows 
    projected onto the cement walls. An impossible tangle of pipes and conduits hangs from above. The only exit lies through a metal door to the
    north.",
    n_to central_supply_door,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor central_supply_door "door" 
    with name 'metal' 'door',
        description [;
            print"It's a thick metal door, currently ";
            open_or_closed(self);
            print ".";
        ],
        door_to [;
            if (parent(self) == hallway_b2) return central_supply; return hallway_b2;
        ],
        door_dir [;
            if (parent(self) == hallway_b2) return s_to; return n_to;
        ],
        found_in central_supply hallway_b2,
    has scenery door openable ~open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room environmental_services "Environmental Services"
    with description [;
        print"This small cement-lined space is filled with various containers of cleaning solvents, brushes, "; 
        !if (mop_bucket in self) print "a mop bucket, ";
        print"and brooms. ";
        print"A small wooden desk is shoved into one corner. ";
        "The exit is to the south. ";
    ],
    s_to hallway_b2,
    has light;

Object environmental_desk "desk" environmental_services
    with name 'desk' 'wooden',
        description [;
        print "It's a small wooden desk, quite stained. ";
        if(PrintContents("On the desk you can see ", self)) "."; else "";
        ],
    has supporter scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room engineering "Engineering"
    with description "This is a small cramped room that smells vaguely of ozone. A metal workbench 
        takes up most of the available space and electrical conduits descend like a massive pipe organ from 
        the ceiling into a large fuse box on the wall. The only exit is east through a plain metal door. ",
        before [;
            Smell:
                "It smells mechanical in here, with a hint of electricity. ";
        ],
    e_to hallway_b2,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room stairwell_m "stairwell_m" 
    with description 
        "This is a dark stairwell just off the main hallway which lies to the south. The word ~Main~ is painter in large letters 
        on the wall here. Concrete stairs zig-zag up and down to 
        the floors above and below.",
    d_to stairwell_b,
    s_to elevator_lobby_m,
    u_to stairwell_2,
    has light stairs,
    ;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room stairwell_2 "stairwell_2"
    with description "This is a dark stairwell just off the main hallway which lies to the south. 
        A large number 2 is painted on the wall. Concrete stairs zig-zag up and down to the floors above and below. ",
        d_to stairwell_m,
        s_to elevator_lobby_2,
        u_to stairwell_3,
    has light stairs;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room stairwell_3 "stairwell_3"
    with description "This is a dark stairwell just off the main hallway which lies to the south. 
        A large number 3 is painted on the wall. Concrete stairs zig-zag down to the floors below. ",
        d_to stairwell_2,
        s_to elevator_lobby_3,
    has light stairs;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room elevator_lobby_3 "elevator_lobby_3" 
    with description [;
        print"This is the third-floor elevator lobby. The elevator doors lie to the south, the 'down' 
        button is embedded in a small panel next to them. The elevator doors are currently ";
        open_or_closed(elevator_doors);
        ". A stairwell lies to the north through an open doorway. The hallway leads east and west. On the wall here you 
        see ~Ward B~ posted in large black letters above an arrow pointing west and a blue line on the floor starts here 
        and leads in that direction as well. ";
    ],
    n_to stairwell_3,
    e_to hallway_3_3,
    s_to elevator_doors,
    in_to elevator_doors,
    w_to hallway_3_1,
    has light; 

Object blue_line "blue line"
    with name 'blue' 'line',
        description "It's a thick blue line painted down the middle of the hallway. ",
        found_in elevator_lobby_3 hallway_3_1 hallway_3_2 b_ward_1 station_b,
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_3_1 "hallway_3_1"
    with description "Here the hallway continues east and west. A service elevator is here in the north wall. 
        A blue line is painted down the middle of the floor. ", 
        e_to elevator_lobby_3,
        w_to hallway_3_2,
        n_to service_elevator_door,
        in_to service_elevator_door,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_3_2 "hallway_3_2" 
    with description "This is the entrance to 'B' Ward which lies down the hallway to the west. The wallpaper here is blue, 
        matching the blue line running down the middle of the floor. The hallway returns to the east and 
        an old service dumbwaiter is here, embedded in the south wall. ",
        e_to hallway_3_1,
        w_to b_ward_1,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room b_ward_1 "Ward B - East"
    with description "The hallway continues east and west. Here the walls are blue to match the line running 
        down the middle of the floor. Patient rooms lie north and south through open doorways. A sign next to the former 
        reads ~33~, one next to the latter reads ~34~.", 
        e_to hallway_3_2,
        w_to station_b,
        n_to room_33,
        s_to room_34,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_33 "Room 33"
    with description "This is an unassuming patient room. The wallpaper is an unsoothing blue and a bed is pushed 
        head-first against the wall. A cast-iron radiator drips and hisses under the window. A doorway 
        leads south back out to the hallway. ",
        s_to b_ward_1,
        cheap_scenery
        4 'cast' 'iron' 'cast-iron' 'radiator' [;
            examine:
            "It's a cast-iron steam radiator. It's hissing and knocking in a vain struggle against the chill. ";
            take, touch, pull, push:
            "I guess third-degree burns are your thing?";
        ],
    has light;

Bed room_33_bed "patient bed" room_33
    with name 'patient' 'bed',
        description "It's a standard hospital bed. ";

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_34 "Room 34"
    with description "This is a single-occupancy patient room. The walls are blue and a single bed is pushed against the wall. 
        There's a doorway leading out to the north. ",
        n_to b_ward_1,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_32 "Room 32"
    with description "This is a single-occupancy room. The walls are blue and a single bed is pushed against the wall.
        The floor is scuffed linoleum and a steaming radiator hisses under a window. The way out is to the north. ",
        cheap_scenery
        4 'cast' 'iron' 'cast-iron' 'radiator' [;
            examine:
            "It's a cast-iron steam radiator. It's hissing and knocking in a vain struggle against the chill. ";
            take, touch, pull, push:
            "I guess third-degree burns are your thing?";
        ],
        n_to station_b,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room station_b "Ward B - Station"
    with description "This is the Ward 'B' nurses' station, lying at the western end of a long east-west corridor. To aid in 
    visitor and patient navigation, the walls here are painted blue and a blue line runs to the east down the 
    middle of the hall towards the main elevators. Open doorways lead to patient rooms to the north and south 
    a sign next to the former read ~31~, the latter ~32~. A large wide waist-high desk occupies the center of the room,
    allowing space for medical staff to chart and dictate. ",
    e_to b_ward_1,
    n_to room_31,
    s_to room_32,
    has light;

Object ward_b_station "nurses' station" station_b
    with name 'nurse' 'station' 'desk',
        description "It's a tall standing desk, square and roughly waist high. It gives the nurses and physicians 
        a surface to update notes and charting. ",
        has supporter scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_31 "Room 31"
    with description "This is an unassuming patient room. The wallpaper is an unsoothing blue and a bed is pushed 
        head-first against the wall. A cast-iron radiator drips and hisses under the window. A doorway 
        leads south back out to the hallway. ",
        s_to station_b,
        cheap_scenery
        4 'cast' 'iron' 'cast-iron' 'radiator' [;
            examine:
            "It's a cast-iron steam radiator. It's hissing and knocking in a vain struggle against the chill. ";
            take, touch, pull, push:
            "I guess third-degree burns are your thing?";
        ],
    has light;

Bed room_31_bed "patient bed" room_31
    with name 'patient' 'bed',
        description "It's a standard hospital bed. ";

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room elevator_lobby_m "Main Elevator Lobby"
    with description [;
        print"This is the main elevator lobby. The elevator doors, currently ";
        open_or_closed(elevator_doors); 
        ", lie to the south, the 'up' and 'down' buttons 
        are embedded in a small panel next to them. An open doorway leads into a stairwell to the north and an open double 
        doorway leads to the east. A sign across the top reads ~Cafeteria~. A long corridor begins here and leads west. ";
        ],
        cheap_scenery
            'cafeteria' 'sign' "It's a large sign over the doorway that reads ~Cafeteria~. ",
        n_to stairwell_m,
        e_to cafeteria,
        w_to hallway_m1,
        s_to elevator_doors,
        in_to elevator_doors,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room cafeteria "Cafeteria"
    with description "This is the hospital cafeteria, dark and empty this time of night. Tables are arranged in rows, inverted chairs 
        stacked on top of them. A long stainless steel buffet serving station lies against one wall, an abandoned cashier
        station against the other. An open doorway leads north into the kitchen and the exit to the main hallway lies to the west. ",
    w_to elevator_lobby_m,
        cheap_scenery
            'table' 'tables//p' [;
                examine:
                "They're standard cafeteria tables. Just like the ones you sit at in study hall. ";
                Take:
                "I don't think so. ";
                Enter:
                "You don't need to climb around on the tables. ";
                Receive:
                "The tables are already occupied by inverted chairs. ";
            ]
            'chair' 'chairs//p' [;
                examine:
                "They're just chairs. ";
                Take:
                "You don't need that. ";
                Enter:
                "You sit down for a moment but restlessly stand up again. ";
            ]
            5 'steam' 'serving' 'table' 'buffet' 'station' [;
                take:
                "Seems unrealistic. ";
                examine: 
                "It's a long aluminum industrial steam table. The wells are all empty. ";
                search:
                "There's nothing hidden in the buffet station. ";
        ],
    n_to kitchen,
    has light;

Object cashier_station "cashier station" cafeteria 
    with name 'cashier' 'station',
        description "It's a metal stand holding an old and very scratched metal cash register. ",
    has scenery;

Object cash_register "cash register" cafeteria 
    with name 'cash' 'register', 
        description [;
            print"It's a very old and weathered metal cash register fixed to the cashier stand. 
            There aren't very many buttons and the numbers on them have largely worn off. ";
            if (cash_drawer has open) "The drawer is currently open. "; else "The drawer is currently closed. "; 
        ],
        describe [;
                    rtrue;
        ],
        before [;
            Take:
                "It seems to be bolted down, likely with sticky-fingered people like yourself in mind. ";
            Insert:
                <<insert noun cash_drawer>>;
            Open:
                <<open cash_drawer>>;
            Close:
                <<close cash_drawer>>;
        ],
    has scenery container open;

Object cash_drawer "cash drawer" cafeteria
    with name 'cash' 'drawer' 'till',
        description [;
            if (self has open) "The till is currently open. "; else "The till is currently closed. ";
        ],
        before [;
            push:
            <<close self>>;
        ],
        after [;
            close:
            give self locked;
        ],
    has container open openable scenery;

Object cash_buttons "buttons" cafeteria 
    with name 'button' 'buttons//p',
        description "They're large mechanical cash register buttons. Whatever labels they once had have largely worn away. ",
        before [;
            push:
                "You randomly poke at the buttons, but nothing useful seems to happen. ";
            take:
                "They're attached to the register. ";
        ],
    has scenery pluralname;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room kitchen "Kitchen"
    with description "Stainless steel prep tables occupy the center of this large room with ovens and stoves arranged along the walls.
    The overhead fluorescent lights are off and it's nearly pitch dark in here, save for the faint green light coming from an 'exit' sign
    over the door to the south. There's a garbage can sitting next to one of the prep tables. ",
    cheap_scenery 
        4 'steel' 'prep' 'tables//p' 'table' [;
            Take:
            "They seem to be bolted to the floor. ";
            Enter:
            "They've been sanitized and you would sully them. ";
            Receive:
            "The tables are very clean and only intended for food products. ";
            examine:
            "They're stainless steel tables occupying the center of the room. ";
        ]
        6 'oven' 'ovens//p' 'stove' 'stoves//p' 'appliance' 'appliances//p' [;
            take:
            "You would need a crew of movers. ";
            enter:
            "You don't need to crawl around in or on that. ";
            receive:
            "You can leave that to the cooks. ";
            examine:
            "They're old but clean industrial appliances arranged around the periphery of the kitchen. ";
        ]
        3 'fluorescent' 'light' 'lights//p' [;
            take:
            "You can't reach them and don't need them. ";
            examine:
            "They're bare fluorescent tubes. They're all off. ";
        ]
        3 'exit' 'light' 'sign' [;
            take:
            "You can't reach it and don't need it. ";
            examine:
            "It's a glowing green 'exit' sign. ";
        ],
    s_to cafeteria,
    has light;

Object garbage_can "garbage can" kitchen
    with name 'garbage' 'rubbish' 'can' 'bin',
        description "It's a large metal garbage can. ",
        before [;
            take:
                "It's too heavy. ";
            search:
                move shrimp to self;
        ],
    has container scenery open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_m1 "hallway_m1"
    with description "This dark hallway continues east and west. A service elevator is here in the north wall and a short wood-panelled 
        corridor begins here and leads south. ",
        after [;
            go:
            if (selected_direction == e_to && ledger in player && FlagIsClear(F_BADDIES_FOLLOWING)) 
            {
                StartTimer(follow_timer, 4);
                SetFlag(F_BADDIES_FOLLOWING);
                print"^You hear a noise in the darkness behind you. You turn to see Dr. Northrup, Nurse Retch, and Vic standing in the 
                shadows down the hallway to the west. The doctor and the nurse look furious but you notice Vic is smiling 
                and has one hand thrust into an inner pocket. Northrup moves first, stepping toward you, his eyes fixed 
                on the ledger you're carrying. The other two follow close behind.^^";
            }
        ],
        s_to lobby_west,
        w_to hallway_m2,
        e_to elevator_lobby_m,
        n_to service_elevator_door,
        in_to service_elevator_door,

    has light;

Object book_cart "book cart" station_b
    with name 'cart' 'push' 'wooden' 'book' 'wheeled',
        description "It's the hospital's miniature lending library. A low wooden cart with shelves built into the sides, 
        you've been pushing it from ward to ward for a month. The pickings are slim currently. There are a few romance novels, 
        and old magazines currently on offer to any interested patients. ",
        before [ dirobj;
            take:
                "It's far too heavy. ";
            push:
                "You should supply a direction. ";
            pull:
                "It would make more sense to push it. ";
            pushdir:
                dirobj = DirPropToFakeObj(selected_direction);
                if (dirobj ==  FAKE_U_OBJ or FAKE_D_OBJ) "You can't push it up or down stairs. ";
                print"Wheels grinding and squeaking, you push the cart to the ";
                print (string) direction_name_array-->selected_direction_index;
                print".^^";
			    <Go dirobj>;
			    move self to real_location;
                rtrue;     
        ],
    has supporter;

Object novels "novels" book_cart
    with name 'novels//p' 'novel' 'book' 'books//p' 'romance',
        description "It's a wide assortment of romance novels. You think about your grandmother who loves these things. ",
        before [;
            read:
                "You flip through one at random. There's a lot of swelling and heaving going on. ";
            take:
                "You have little interest in romance novels. ";
        ],
    has scenery pluralname;

Object magazines "magazines" book_cart
    with name 'magazines//p' 'magazine' 'time' 'people' 'newsweek',
        description "It's a motley collection of old magazines. You see issues of 'Time', 'People',
            and 'Newsweek' among the stacks.",
        before [;
            take: 
                "You don't need an old magaine that's not 'Teen Beat' or 'Seventeen'.";
            read:
                print"You flip through a random magazine. ";
                read_news();
                rtrue;
        ],
    has scenery pluralname;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_m2 "hallway_m2"
    with description "The hallway here continues east and west. An old dumbwaiter is embedded in the south wall. ",
        before [;
            go:
            if (selected_direction == u_to && ladder in self) 
                if (ladder hasnt open) { "The ladder is folded up. "; }
                else
                {
                print"You climb the creaky metal ladder nearly to the top. Once there, you are able to 
                grab onto some sturdy pipes that you see through the gap in the askew tiling and pull yourself into the ceiling above. 
                Victorious, you think back to P.E. class freshman year when you couldn't climb a knotted rope in gym class and 
                Becky Harris laughed at you. Eat it Becky!^^";
                PlayerTo(ceiling_01);
                rtrue;
                }
            examine:
            if (selected_direction == u_to) "You notice one of the large tiles in the drop-ceiling is slightly out of alignment. You can see darkness behind it. ";
            if (selected_direction == d_to) "You see a fairly nice dark blue carpet. ";
        ],
    e_to hallway_m1,
    w_to admin_hallway,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room lobby_west "Lobby West"
    with description "This is the western extension of the main lobby which lies to the east. Decorative tile murals cover
        the walls and a corridor begins here and extends to the north. A glass door to the west is labeled ~Security~. ",
    n_to hallway_m1,
    cheap_scenery
        'mural' 'murals//p' [;
            take:
            "I think you would need to take the whole wall. ";
            examine:
            "They're made of tiny pieces of colored glass tiles. They depict scenes of doctors and nurses standing over 
            grateful bedridden patients. ";
        ],
    e_to main_lobby,
    w_to security_door,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room security_office "Security Office" 
    with description [;
            print"This cluttered office smells vaguely of old pizza and sour-cream potato chips. A bank of 
            closed-circuit monitors lines one wall, although none of them are turned on or seem to be functional. A long desk
            fronts the monitors. It's ringed with water stains and burger wrappers. ";
            print"The exit lies to the east.^";
            if (buzz in security_chair) {
                if (buzz hasnt encountered) {
                    print"^Buzz, the head of security, is sitting at the desk. He's engrossed in the newspaper and 
                        intermittently reaches over and blindly plucks a shrimp from a bowl and plops it into his mouth.^"; 
                    give buzz encountered;
                    StartDaemon(buzz);
                } else {
                    "^Buzz is sitting here, reading the newspaper. ";
                }
            }
        ],
        cheap_scenery 
            5 'water' 'rings//p' 'ring' 'stain' 'stains//p' "They're water stains, likely permanent. Use a coaster, people. "
            4 'burger' 'wrapper' 'wrappers//p' 'garbage' [;
                take:
                "It's literally garbage. ";
                examine:
                "They're greasy wadded up paper wrappers. ";
            ]
            8 'closed' 'circuit' 'monitor' 'monitors//p' 'television' 'televisions//p' 'tv' 'tvs//p' [;
                take:
                "They're fixed in place and quite heavy. ";
                examine:
                "It's a low bank of closed-circuit monitors. None of them appear to work. ";
            ],
        before    [;
            smell:
                if (noun == 0) "It smells like onions, garlic, and old cheese. ";
        ],
        e_to security_door,
    has light;

Object security_desk "security desk" security_office
    with name 'security' 'desk',
        description"It's a long desk facing the bank of closed-circuit monitors. ",
    has supporter scenery;

OnChair security_chair "metal chair" security_office
    with name 'metal' 'chair',
        description "It's a standard metal and plastic chair. ",
        mass 25,
        describe [;
            if (self in security_office)
                rtrue;
        ];

 !%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor security_door "security door"
     with name 'metal' 'security' 'door',
        description [;
            print"It's an unassuming metal door, currently ";open_or_closed(self);".";
        ],
        door_to [;
            if (parent(self) == lobby_west) return security_office; return lobby_west;
        ],
        door_dir [;
            if (parent(self) == lobby_west) return w_to; return e_to;
        ],
        found_in lobby_west security_office,
    has scenery door openable ~open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room main_lobby "Main Lobby"
    with description [;
            print"This is the main lobby and hospital entrance. The wall to the south is mostly glass including the large double doors 
            leading out. The windows are partially iced over and through them you can see gales of wind-driven snow blowing sideways. 
            The lobby continues back to the west. An information desk occupies the north wall.^";
            !if(mabel in main_lobby) "^^Mabel, a hospital volunteer, is currently staffing the information desk. "; "";
        ],
        after [;
            go:
                if (selected_direction == n_to) {
                    give main_lobby_doors ~open;
                    print"You push through the doors and with a hiss they close behind you.^";
                }
        ],
        cheap_scenery
        'information' 'desk' "It's a round wooden desk with the word 'Information' emblazoned across the front. "
        3 'lobby' 'windows//p' 'window' "They're icing over and difficult to see through. Through them you can 
            intermittently catch a glimpse of the blizzard outside. ",
        w_to lobby_west,
        s_to main_lobby_doors,
        each_turn [;
            if (book_cart in self && FlagIsClear(F_HAVE_FLASHLIGHT)) 
            {
                SetFlag(F_HAVE_FLASHLIGHT);
                move flashlight to player; 
                print"^Mabel sees the cart you're pushing. ~Let's see here. Which one of these trashy things haven't 
                I read?~ She giggles a bit and plucks one from the cart. ~Ooh, look at the pecs on that hunk of sugar,~ 
                She picks up a book titled 'Heaving Waves of Passion'.^^
                ~Ooh, I almost forgot. Take this.~^
                She pulls a flashlight from the folds of her layers and hands it to you.^^
                ~You may need this tonight if the blizzard causes a brown-out. I sure do wish I'd gotten myself 
                out of here and home before they closed the roads.~^";
            }
        ]
    has light;

 !%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor main_lobby_doors "lobby doors"
     with name 'lobby' 'double' 'doors' 'door',
        description "They're a pair of glass doors. ",
        door_to [;
            if (parent(self) == main_lobby) return outside; return main_lobby;
        ],
        door_dir [;
            if (parent(self) == main_lobby) return s_to; return n_to;
        ],
        found_in main_lobby outside,
    has scenery door openable ~open pluralname;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Global outside_timer = 0;
Room outside "Outside"
    with description "You're standing outside the main hospital entrance. The freezing wind bites into your skin and flecks of ice 
        sand-blast your cheeks. You're having a difficult time catching a breath in the intensely cold air. ",
        after [;
            go:
                if (selected_direction == s_to) {
                    print"You push through the doors and with a hiss they close behind you.^";
                    give main_lobby_doors ~open;
                    outside_timer = 0;
                }
        ],
        each_turn [;
            switch(outside_timer)   {
                1:  print"You're not sure how long you can stand the cold.^";
                2:  print"You notice that your lips seem to be turning blue.^";
                3:  print"Unable to stand the cold any longer, you collapse in a heap of snow. Unfortunately, you seem to have 
                    died of hypothermia. "; deadflag = true;
                }
                outside_timer++;
            ],
    n_to main_lobby_doors,
    has light; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room admin_hallway "Administration"
    with description "This is the western end of a long east-west hallway. It's a bit better maintained 
        than the others and warmer colors accentuate the walls. The overhead fluorescent tubes are replaced 
        here by round light fixtures and the floor is covered with blue carpet. There's a plain wooden door 
        to the north with a small placard mounted next to it and a set of french glass doors lie to the west. ",
    cheap_scenery
    3 'brass' 'placard' 'sign' [;
        take:
        "If you want one, you should get an MBA. ";
        read:
        "~Sid Jorry, VP/CFO~";
        examine:
        "It's a small brass placard mounted next to the door. It reads ~Sid Jorry, VP/CFO~.";
    ]
    'blue' 'carpet' "It's dark blue carpet. It looks relatively new. ",
    before [;
        go:
        if (selected_direction == n_to && FREDDY_ASLEEP == false) "Freddy stops you. ~Whoa, dude. Nobody goes in there until the cops get here. 
        Northrup's orders.~";
    ],
    e_to hallway_m2,
    n_to jorry_door,
    w_to french_doors,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
OnChair folding_chair "metal folding chair" admin_hallway
    with name 'metal' 'folding' 'chair',
        description "It's a standard metal folding chair. ",
        invent "",
    has scenery;

 !%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor french_doors "french doors" 
    with name 'french' 'double' 'door' 'doors//p',
        description [;
            print"They're a pair of glass french doors, currently ";
            open_or_closed(self);
            print ".";
        ],
        door_to [;
            if (parent(self) == admin_hallway) return northrup_anteroom; return admin_hallway;
        ],
        door_dir [;
            if (parent(self) == admin_hallway) return w_to; return e_to;
        ],
        found_in admin_hallway northrup_anteroom,
    has scenery door openable ~open pluralname;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor jorry_door "office door" 
    with name 'door' 'wooden' 'office',
        description [;
            print"It's a plain wooden office door, currently ";
            open_or_closed(self);
            print ".";
        ],
        before [;
            open:
            if (real_location == admin_hallway && FREDDY_ASLEEP == false) 
            "Freddy stops you. ~Sorry, dude. Nobody goes in until the cops get here. ";
        ],
        door_to [;
            if (parent(self) == admin_hallway) return jorry_office; return admin_hallway;
        ],
        door_dir [;
            if (parent(self) == admin_hallway) return n_to; return s_to;
        ],
        found_in admin_hallway jorry_office,
    has scenery door openable ~open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room jorry_office "Office" 
    with description [;
        print"This is a typical office befitting a less-than-top-level executive. A desk stands here";
        if (jorry_chair in self) { print " as well as a rolling leather executive chair"; }
        print". Non-threatening paintings of flowers and bowls of fruit adorn the walls. A large glass aquarium sits on 
        a stand directly behind the desk. Sand covers the bottom and a snake is coiled around a bleached twisted 
        branch at one end. A squat black safe sits in one corner.
        ^^The former Sid Jorry is sitting in the chair and slumped over the desk.
        His eyes, thankfully, are closed.^";
        if (self hasnt visited) "^It appears that somone has been in here already and cleaned out the room. 
            There's no trace of any papers or documents. ";
    ],
    cheap_scenery
    4 'painting' 'paintings//p' 'picture' 'pictures//p' [;
        examine:
        "They're generic prints of flowers and fruit. You feel both soothed and insulted just looking at them. ";
        take:
        "You have no need for mediocre art. ";
    ]
    3 'combination' 'lock' 'dial' [;
        examine:"It's a dial with the numbers 1 through 35 around the periphery. A large hole has been drilled in it
            rendering it useless. ";
        turn:
            "It's broken and won't turn. ";
    ]
    3 'wrought' 'iron' 'stand' "It's a wrought iron stand holding up the aquarium. "
    3 'reticulated' 'python' 'snake' [;
        examine:
        "'Louanne' is a two-foot long reticulated python although you have no way of knowing that, having no 
        particular expertise in snakes. She's seems content to stay where she is but you would rather keep your distance. ";
        take:
        "Nope. Just no. ";
    ]
    3 'bleached' 'branch' 'stick' [;
        examine:
        "It's a tortuous piece of bleached wood. There's currently a snake wrapped around and sleeping on it. ";
        take:
        "Not with the snake there, nosirree. ";
    ],
    each_turn [;
        print (string) random("^Louanne, curious, flicks her forked tongue at you.^",
            "^Louanne stares at you through yellow reptillian eyes.^", "", "");
    ],
    s_to jorry_door,
    has light;

Object aquarium "aquarium" jorry_office
    with name 'aquarium' 'tank',
        description [;
            print"It's a large aquarium with a plastic cover over the top. A thick layer of sand lines the bottom.
            A twisty bleached piece of branch sits in one end, a large snake is coiled motionless on and around it. ";
            if (rock in self) "A large decorative rock sits in the sand in the other end. "; "";
        ],
    has scenery transparent container open;

Object jorry_note "note"
    with name 'note' 'paper' 'letter',
        description "It appears to be a hastily scrawled note. ",
        before [;
            read:
            "~I have evidence that Walter Northrup and others have been embezzling funds from St. Asterix for
            years. Since powerful people are involved and I cannot be completely confident of my safety, I have hidden 
            a ledger containing the receipts in my safe. However, knowing that my own safe would be immediately searched, 
            I have swapped my safe with the one in Northrup's office. The combination is 23-12-18.~^At the 
            bottom of the note you see the initials ~S.J.~";
            take:
            if (self in rock) move self to location;
        ],
    mass 1,
    has item;

InChair jorry_chair "office chair" jorry_office
    with name 'chair' 'seat' 'rolling' 'leather',
        description "It's a faux-leather rolling executive chair. ",
        mass 24,
        describe [;
            if (self in jorry_office) rtrue;
        ],
        before [;
            take:
                "It's too heavy what with the dead body in it. ";
            pushdir:
                "Between the carpet and the dead body you're not rolling the chair anywhere. ";
        ],
    has supporter enterable static;

Object jorry_safe "safe" jorry_office 
    with name 'safe',
        description [ ;
            print"It's a squat black metal safe, about two feet on each side. It's currently ";
            open_or_closed(self); " and there's a large hole drilled through the combination lock. ";
        ],
        before [;
            take:
                "It's far too heavy. ";
        ],
    has container openable scenery open;

Object jorry_desk "office desk" jorry_office
    with name 'desk' 'office',
        description "It's a standard executive desk, wooden with a glass top. A single drawer runs across the knee well. ",
        before [;
            open:
                <<open jorry_drawer>>;
            close:
                <<close jorry_drawer>>;
        ],
    has supporter scenery;

Object jorry_drawer "desk drawer" jorry_office  
    with name 'drawer',
        description [;
            print"It's a long narrow drawer, currently "; 
            open_or_closed(self);
            ".";
        ],
        describe [; rtrue; ],
    has scenery container openable ~open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room northrup_anteroom "Anteroom"
    with description [;
        print"This is a small secretarial space off of the main office to the south. A small desk ";
        if (secretary_chair in self) print"and chair are here"; else print "is here";
        print". Glass french doors lead back east to the hallway and a standard wooden door, currently ";
        open_or_closed(northrup_door);
        print", leads south. There's a brass placard next to the door.^"; 
        !(if)(northrup in self && northrup_door has ~open) "and light coming from under it. "; else ".";
        ],
        cheap_scenery
        3 'brass' 'placard' 'sign' [;
            examine:
            "It's a brass placard. It reads ~Walter Northrup, M.D., President/CEO~.";
            take:
            "It's firmly fixed to the wall. ";
            read:
            "~Walt Northrup, M.D., President/CEO~";
        ],
        e_to french_doors,
        s_to northrup_door,
    has light;

Object secretary_desk "office desk" northrup_anteroom
    with name 'wooden' 'secretary' 'desk',
        description"It's an unassuming wooden desk. ",
    has supporter scenery;

OnChair secretary_chair "secretary chair" northrup_anteroom
    with name 'office' 'chair',
        description "It's a standard office chair.",
        mass 20,
        describe [;
            if (self in northrup_anteroom) rtrue;
        ];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor northrup_door "office door" 
    with name 'door' 'dark' 'wooden' 'office',
        description [;
            print"It's a thick dark wooden door, currently  ";
            open_or_closed(self);
            print ".";
        ],
        door_to [;
            if (parent(self) == northrup_anteroom) return northrup_office; return northrup_anteroom;
        ],
        door_dir [;
            if (parent(self) == northrup_anteroom) return s_to; return n_to;
        ],
        found_in northrup_anteroom northrup_office,
    has scenery door openable ~open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room northrup_office "Northrup's Office"
    with description[;
        print"This is a large office belonging to Dr. Walter Northrup, the president and CEO of the hospital. 
        An imposing mahogany desk dominates the room and large wooden bookshelves line the walls. A heavy brass floor lamp 
        stands in one corner. The walls are adorned with the requisite paintings and the floor is covered in thick beige carpet.
        A black safe squats in one corner and a file cabinet in another. The room smells vaguely of pipe smoke";
        if (northrup_chair in self) " and a plush leather executive chair is here next to the desk. "; else ".";
        ],
        cheap_scenery
        6 'bookcase' 'bookshelf' 'shelves//p' 'shelf' 'bookcases//p' 'bookshelves//p' [;
            examine:
            "They're dark wooden shelves, seemingly hand-made to match the desk. They're full of 
            numerous books which you don't need. ";
            take:
            "Seems unlikely. ";
        ]
        5 'medical' 'book' 'books' 'journal' 'journals' [;
            examine:
            "You see numerous books and medical journals. Nothing that appeals to you. ";
            take:
            "While you could stand to be a bit more well read, you don't need these particular books. ";
            read:
            "You already know everything you need to know about the pineal gland. ";
        ]
        'painting' 'paintings' [;
            examine:
            "They're boring pictures of landscapes and seascapes. ";
            take:
            "They're lovely but you don't need them. ";
        ]
        3 'thick' 'beige' 'carpet' "It's a standard low pile beige carpet. ",
        before [;
            smell:
                if (noun == 0) "It smells like an attic full of old books and expensive pipe tobacco. ";
            go:
            if (selected_direction == u_to)
            {
                if (player notin file_cabinet) "You can't reach the ceiling from down here. ";
            }
        ],
        each_turn [;
            if (IndirectlyContains(self, northrup) && IndirectlyContains(self, nurse_retch)) 
            {
                print"^You seem to have interupted the start of an animated conversation between Dr. Northrup and 
                Nurse Retch. They abuptly clam up as you enter.^^
                ~Ahh, the candystriper I've heard about. The one poking around into things that have nothing to do with her;
                things she doesn't understand; things she may be misinterpreting?~^^
                He smiles for a moment and then his face hardens.^^
                ~I don't know what you think is happening here, young lady. But I believe you need a letter testifying 
                to your successful completion of your volunteer assignment here at St. Asterix. Now that poor Mr. Jorry 
                has passed away, that letter would come from me. So if I were you, I'd finish out my time this evening and 
                be done with it.~^^
                He turns to Nurse Retch.^^
                ~I think we'd best have a talk with our mutual friend. And you, my dear, need to leave.~^^
                Northrup motions to the door and you step out into the anteroom. He and Nurse Retch follow and Northrup 
                closes the locks the office door behind him. He and Retch quickly leave to the east and disappear 
                down the dark hallway.^^";
                remove nurse_retch;
                remove northrup;
                give northrup_door ~open;
                give northrup_door locked;
                SetFlag(F_NORTHRUP_OUT_OF_OFFICE);
                PlayerTo(northrup_anteroom); 
            }
        ],
    n_to northrup_door,
    has light;

Object file_cabinet "file cabinet" northrup_office
    with name 'metal' 'gray' 'file' 'cabinet' 'drawer' 'drawers//p',
        description"It's a standard gray metal file cabinet with three large drawers. ",
        react_before [;
            go:
            if(selected_direction == u_to && player in self)
            {
                print"Tiptoeing carefully on the file cabin, you are able to reach up through the gap in the ceiling
                tiles. Grabbing hold of a sturdy pipe, you pull yourself up and into the dark space above the office,^^";
                PlayerTo(ceiling_05);
                rtrue;
            }
            if(player in self) 
            {
                PlayerTo(northrup_office, 1);
                print"(first getting down from the file cabinet)^";
            }
        ],
        before [;
            enter, climb:
            PlayerTo(self, 1);
            "With some difficulty, you awkwardly clamber onto the file cabinet. ";
            exit, getoff:
            PlayerTo(northrup_office, 1);
            "You lower yourself from the file cabinet. ";
            open:
            "The drawers are all securely locked. ";
            take:
            "It's far too heavy. ";
        ],
    has supporter scenery enterable;

Object northrup_desk "desk" northrup_office
    with name 'mahogany' 'desk',
        description"It's carved from mahogany and looks very old and very expensive. ",
        before [;
            take:
                "You're just begging for a hernia. ";
        ],
    has supporter scenery;

InChair northrup_chair "leather chair" northrup_office
    with name 'plush' 'leather' 'executive' 'chair',
        description"It's a large plush executive leather chair. ",
        before [;
            take:
                "It's too heavy. ";
        ],
        describe [;
            if (self in northrup_office) rtrue;
        ],
    has scenery;

Object northrup_lamp "floor lamp" northrup_office
    with name 'lamp' 'floor',
        description [;
            print"It's a hefty floor lamp, currently ";
            if (self has on) "on."; else "off.";
        ],
        before [;
            take:
                "It's not a battered trusty portable light source. ";   
        ],
    has scenery switchable;

Object northrup_safe "safe" northrup_office 
    with name 'safe',
        description [ ;
            print"It's a squat black metal safe, about two feet on each side. It's currently ";
            open_or_closed(self);
            ". A dial is embedded in the safe door. The numbers 1 through 35 circle it. The dial 
            is currently set to ",SAFE_CURRENT,".";
        ],
        before [;
            take:
                "It's far too heavy. ";
        ],
    has container locked openable scenery ~open;

Object combo_dial "dial" northrup_office
    with
        name 'dial' 'combination',
        description [;
            print"It's the dial to a combination lock. The numbers 1 through 35 circle the outer
            diameter and it's currently set to ",SAFE_CURRENT,".^";
        ],
        before [;
            TurnClockwiseTo:
            if (second < 1 || second > 35) "The numbers only go from 1 to 35.";
            print"You turn the dial clockwise to ",second,".^";
            SAFE_CURRENT = second;
            if (SAFE_CYCLE == 0 && second == SAFE_1) 
                {
                    SAFE_CYCLE++;
                    if (SAFE_CYCLE == 3)
                    {
                        give northrup_safe ~locked;
                        "You hear a faint ~click~ from within the safe door. ";
                    } 
                    rtrue;                  
                }
            if (SAFE_CYCLE == 2 && second == SAFE_3) 
                {
                    SAFE_CYCLE++;
                    if (SAFE_CYCLE == 3)
                    {
                        give northrup_safe ~locked;
                        "You hear a faint ~click~ from within the safe door. ";
                    } 
                    rtrue;
                }
            SAFE_CYCLE = 0;
            rtrue;
            TurnCounterclockwiseTo:
            if (second < 1 || second > 35) "The numbers only go from 1 to 35.";
            print"You turn the dial counterclockwise to ",second,".^";
            SAFE_CURRENT = second;
            if (SAFE_CYCLE == 1 && second == SAFE_2) 
                {
                    SAFE_CYCLE++;
                    if (SAFE_CYCLE == 3)
                    {
                        give northrup_safe ~locked;
                        "You hear a faint ~click~ from within the safe door. ";
                    } 
                    rtrue; 
                }
            SAFE_CYCLE = 0;
            rtrue;
        ],
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room elevator_lobby_2 "elevator_lobby_2" 
    with description [;
        print"This is the second-floor elevator lobby. The elevator doors lie to the south, the 'up' and 'down' 
        buttons embedded in a small panel next to them. The elevator doors are currently ";
        open_or_closed(elevator_doors); 
        ". A stairwell lies to the north through an open doorway. On the wall 
        here you see ~Radiology~ posted in large black letters above an arrow pointing east. Above an arrow 
        pointing west you see a large red 'A' and a red line on the floor starts here and leads in that direction as well. ";
    ],
    n_to stairwell_2,
    e_to hallway_2_3,
    !e_to radiology,
    s_to elevator_doors,
    in_to elevator_doors,
    w_to hallway_2_1,
    has light;

Object red_line "red line"
    with name 'red' 'line',
        description "It's a thick red line painted down the middle of the hallway. ",
        found_in elevator_lobby_2 hallway_2_1 hallway_2_2 a_ward_1 station_a,
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_2_3 "hallway_2_3" 
    with description "This long dim hallway continues east towards the radiology suite and west 
        toward the elevators. ",
    w_to elevator_lobby_2,
    e_to radiology,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room radiology "Radiology"
    with description "This is the eastern end of a long east-west hallway. There's an open archway to the south with a large sign 
        above reading ~Imaging~. An open doorwary leads north as well. A sign posted above to it reads ~MRI.~. ",
    w_to hallway_2_3,
    n_to mri_anteroom,
    s_to x_ray,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room mri_anteroom "MRI Anteroom"
    with description "This small room serves as the prep area and control room for the hospital's MRI scanner. 
        The west wall contains a window of thick safety glass. Beneath the window squats a large control desk and monitor. 
        A red warning sign is fixed to the wall. A metal and reinforced-glass door leads west into the MRI suite itself. ",
        cheap_scenery
        'warning' 'sign' [;
            examine:
            "In large red letters it reads: ~Danger, Strong Magnetic Field~. Below that is a pictograph of a horseshoe magnet
            with intimidating lightning bolts coming out of it. Below that it reads ~This Magnet is Always ON - No loose 
            metal objects. Objects made from or containing ferrous metals should not be taken into this room. Serious 
            injury or property damage may result~. ";
            read:
            "~Danger, Strong Magnetic Field~. Below that is a pictograph of a horseshoe magnet
            with intimidating lightning bolts coming out of it. Below that it reads ~This Magnet is Always ON - No loose 
            metal objects. Objects made from or containing ferrous metals should not be taken into this room. Serious 
            injury or property damage may result~. ";
        ],
         after [;
            go:
            if (selected_direction == e_to) 
            {
                give scanner_door ~open;
                print"The glass door swings closed behind you.^^";
            }
        ],
        s_to radiology,
        w_to scanner_door,
        has light;

Object control_desk "control desk" mri_anteroom
    with name 'control' 'desk' 'panel',
        description"A CRT terminal with attached keyboard sits on the desk next to a bank of buttons and switches. 
        The important ones seem to be a green one labeled ~Scan~ and a red one labeled ~Stop~. ",
        describe [;
            rtrue;
        ],
    has supporter scenery transparent;

Object crt_terminal "terminal" control_desk
    with name 'crt' 'tv' 'video' 'crt' 'terminal' 'monitor' 'screen',
        description [;
            print"It's a standard amber CRT monitor. You don't understand anything that's currently displayed on the screen";
            if (green_button.time_left > 0) " but you do see what appears to be a countdown with the number ",green_button.time_left," displayed. ";
                else ".";
        ],
        before [;
            read:
                <<examine self>>;
        ],
    has static concealed;

Object red_button "red button" mri_anteroom
    with name 'red' 'button',
        description"It's a large red button. It's labelled ~Arm~. ",
        before [;
            push:
            "The mechanical whining seems to intensify. ";
        ],
    has scenery;

Object green_button "green button" mri_anteroom
    with name 'green' 'button',
        description"It's a large green button. It's labelled ~Scan~. ",
        time_left,
        before [;
            push:
            if (self.time_left > 0) "Nothing seems to happen. ";
            StartTimer(self, 5);
            "The mechanical whining becomes louder and the banging more insistent. The CRT monitor blooms to life and a countdown appears 
            with the number 5 displayed. ";
        ],
        time_out [;
            if (metal_cart notin mri_scanner) { mri_no_cart(); rtrue; }
            if (real_location == mri_anteroom) { mri_cart_anteroom(); rtrue; }
            if (player in mri_hatch && mri_hatch has open) { mri_cart_hatch_open(); rtrue; }
            if (player in mri_hatch && mri_hatch hasnt open) { mri_cart_hatch_closed(); rtrue; }
            if (player in mri_scanner) { mri_cart_unprotected(); rtrue; }
            print"^^Suddenly, from somewhere in the building you hear a horrible cacophany of metal smashing metal 
            and glass shattering. It's quickly discovered that the MRI machine has been broken beyond repair. 
            While there is no direct proof that this was your doing, you cannot risk being blamed for the destruction 
            of a very expensive MRI scanner. You decide it's best to now worry about your volunteering letter and simply 
            slip away. ";
            deadflag = 3;
            rtrue;
        ],
    has scenery;

 !%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor scanner_door "thick glass door"
     with name 'glass' 'thick' 'door',
        description [;
            print"It's a complicated non-magnetic metal and glass door, currently ";open_or_closed(self);".";
        ],
        door_to [;
            if (parent(self) == mri_anteroom) return mri_scanner; return mri_anteroom;
        ],
        door_dir [;
            if (parent(self) == mri_anteroom) return w_to; return e_to;
        ],
        react_after [;
            go:
                if (noun == e_obj || noun == w_obj)  { give self ~open; print"As you pass through the door it swings closed behind you.^"; }
        ],
        found_in mri_scanner mri_anteroom,
    has scenery door openable ~open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room mri_scanner "MRI Suite"
    with description "This white sterile room is largely empty save for the large white cylinder of the MRI machine. A similarly
        white long narrow table is positioned at the opening, the bore of which is about two feet across. As in the other room, 
        a large red warning sign is posted to the wall warning of the dangers of the strong metallic field. 
        A thick window in the east wall allows a view into the control room. There's a small hatch in the floor next to the scanner, presumably to 
        allow service access. You can return east to the control. ",
        cheap_scenery
         'warning' 'sign' [;
            examine:
            "In large red letters it reads: ~Danger, Strong Magnetic Field~. Below that is a pictograph of a horseshoe magnet
            with intimidating lightning bolts coming out of it. Below that it reads ~This Magnet is Always ON - No loose 
            metal objects. Objects made from or containing ferrous metals should not be taken into this room. Serious 
            injury or property damage may result~. ";
            read:
            "~Danger, Strong Magnetic Field~. Below that is a pictograph of a horseshoe magnet
            with intimidating lightning bolts coming out of it. Below that it reads ~This Magnet is Always ON - No loose 
            metal objects. Objects made from or containing ferrous metals should not be taken into this room. Serious 
            injury or property damage may result~. ";
        ],
        after [;
            go:
            if (selected_direction == w_to) 
            {
                give scanner_door ~open;
                print"The glass door swings closed behind you.^^";
            }
        ],
        e_to scanner_door,
    has light;

Object mri_machine "MRI machine" mri_scanner
    with name 'mri' 'tube' 'm.r.i.' 'MRI' 'machine' 'scanner',
        description "It's a large white box with a large white tube attached. The bore of the tube is about two feet across. ",
        before [;
            enter:
                "You enter the narrow dark tube, immediately start to hyperventilate, and then quickly climb out. ";
            take:
                "You're really quite the comedian. ";
        ],
    has scenery container enterable open;

Bed mri_table "exam table" mri_scanner 
    with name 'exam' 'table' 'white',
        description "It's a white exam table that would typically slide into the MRI tube. ",
    has scenery; 

Object mri_hatch "hatch" mri_scanner
    with 
        parse_name [ w1 w2 ;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'compartment' && w2 == 'door') return 2;
            if (w1 == 'hatch' or 'door') return 1;
            if (w1 == 'hatch' && w2 == 'door') return 2;
            if (w1 == 'compartment' or 'space') return 1;
        ],
        name 'hatch' 'compartment',
        inside_description [;
            if (self has open) "You're sitting in a small compartment under the floor of the MRI suite, surrounded 
            by wiring conduits and pipes. ";
            "You're crouched in a small dark compartment beneath the floor of the MRI suite, hemmed in by wiring conduits
            and pipes. ";
        ],
        before [;
            open:
            if (self has open) "It's already open. ";
            give self open;
            if (player in mri_scanner) "You open the hatch, revealing a small empty space surrounded by wires and pipes. ";
                "You open the hatch. ";
            enter:
            print"You climb down, squeezing yourself into the small compartment.^";
            PlayerTo(self, 1);
            rtrue;
        ],
        description [;
            if (self has open) "Through an open hatch in the floor you can see a small dark space full of wires
            and pipes. ";
            "It's a closed hatch in the floor next to the MRI scanner. ";
        ],
    has scenery light container enterable openable ~open; 

Object metal_cart "cart" mri_scanner !central_supply
    with name 'metal' 'cart',
        description "It's an old steel push cart, battered and rusted in spots. It's loaded with 
        large metal oxygen cannisters. ",
            before [ dirobj;
            take:
                "Love the optimism but no. ";
            push:
                "You should supply a direction. ";
            pull:
                "It would make more sense to push it. ";
            pushdir:
                dirobj = DirPropToFakeObj(selected_direction);
                if (dirobj ==  FAKE_U_OBJ or FAKE_D_OBJ) "You can't push it up or down stairs. ";
                print"Wheels grinding and squeaking, you shove the heavy cart to the ";
                print (string) direction_name_array-->selected_direction_index;
                print".^^";
			    <Go dirobj>;
			    move self to real_location;
                rtrue;   
            receive:
            "The cart is already full. ";  
        ],
    has supporter;

Object oxygen "oxygen cannisters" metal_cart
    with name 'oxygen' 'cannister' 'cannisters//p',
        article "a bunch of",
        description "You see a collection of maybe 20 large metal oxygen cannisters. ",
        before [;
            take:
                "Each cannister on its own is too heavy and awkward. ";
        ],
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room x_ray "X-Ray Suite" 
    with description"This white-tiled room is occupied by a metal exam table positioned below the x-ray camera 
        hanging down from rails in the ceiling. White translucent viewing boxes are mounted to one wall; these
        are currently turned off. The exit lies through an 
        open arch to the north. ",
        n_to radiology,
        has light;

Bed x_ray_table "x-ray table" x_ray 
    with name 'metal' 'exam' 'table',
        description"It's a very uncomfortable looking metal exam table. ",
        before [;
            take:
                "It's securely bolted to the floor. ";
        ];

Object x_ray_camera "camera" x_ray
    with name 'x-ray' 'xray' 'xr' 'camera',
        description"It's bulky metal and glass x-ray camera, fixed to rails in the ceiling for easier positioning above the patient. ",
        before [;
            take:
                "It's attached to the rails in the ceiling. ";
            push, pull:
                "You push and pull the camera back and forth along the rails. Wheee! ";
        ],
    has scenery;

Object rails "rails" x_ray 
    with name 'rail' 'rails//p',
        description"They're metal rails embedded in the ceiling. They allow for easy positioning of the xray camera. ",
        before [;
            take:
                "You can't reach them and don't need them. ";
        ],
    has scenery;

Object viewing_boxes "viewing boxes" x_ray 
    with name 'viewing' 'box' 'boxes//p',
        description [;
                print"It's a row of light boxes used to view x-rays. They're currently ";
                if (self has on) {
                    print"on and flickering with a dull white light. ";
                }   else {
                    print"turned off. ";
                }
                "There's a switch on the side. ";
        ],
        before [;
            switchon:
                give self on;
                "You flick the switch and the viewing boxes flicker alight.";
            switchoff:
                give self ~on;
                "You flick the switch and the viewing boxes flicker off. ";
        ],
    has scenery switchable;

Object box_switch "switch" x_ray
    with name 'switch' 'toggle',
        description"It's a metal toggle switch. ",
        before [;
            flick:
                if (viewing_boxes has on)   <<switchoff viewing_boxes>>; else <<switchon viewing_boxes>>;
            take:
                "It's fixed to the viewing boxes. ";
        ],
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_2_1 "hallway_2_1"
    with description "Here the hallway continues east and west. A service elevator is here in the north wall. 
        A red line is painted down the middle of the floor. ",
        e_to elevator_lobby_2,
        w_to hallway_2_2,
        n_to service_elevator_door,
        in_to service_elevator_door,
    has light;  

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_2_2 "hallway_2_2" 
    with description "This is the entrance to 'A' Ward which lies down the hallway to the west. The wallpaper here is red, 
        matching the red line running down the middle of the floor. The hallway returns to the east and 
        an old service dumbwaiter is here, embedded in the south wall. ",
        e_to hallway_2_1,
        w_to a_ward_1,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room a_ward_1 "Ward A - East"
    with description "The hallway continues east and west. Here the walls are red to match the line running 
        down the middle of the floor. Patient rooms lie north and south through open doorways. A sign next to the former 
        reads ~23-A~, one next to the latter reads ~24-A~.",
        cheap_scenery
        'south' 'sign' "It reads ~24-A~"
        'north' 'sign' "It reads ~23-A~", 
        e_to hallway_2_2,
        w_to station_a,
        n_to room_23,
        s_to room_24,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_23 "Room 23"
    with description "This is an unassuming patient room. The wallpaper is an unsoothing red and a bed is pushed 
        head-first against the wall. A cast-iron radiator drips and hisses under the window. A doorway 
        leads north back out to the hallway. ",
        s_to a_ward_1,
        cheap_scenery
        4 'cast' 'iron' 'cast-iron' 'radiator' [;
            examine:
            "It's a cast-iron steam radiator. It's hissing and knocking in a vain struggle against the chill. ";
            take, touch, pull, push:
            "I guess third-degree burns are your thing?";
        ],
    has light;

Bed room_23_bed "patient bed" room_23
    with name 'patient' 'bed',
        description "It's a standard hospital bed. ";

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_24 "Room 24"
    with description "This is an unassuming patient room. The wallpaper is an unsoothing red and a bed is pushed 
        head-first against the wall. A cast-iron radiator drips and hisses under the window. A doorway 
        leads north back out to the hallway. ",
        n_to a_ward_1,
        cheap_scenery
        4 'cast' 'iron' 'cast-iron' 'radiator' [;
            examine:
            "It's a cast-iron steam radiator. It's hissing and knocking in a vain struggle against the chill. ";
            take, touch, pull, push:
            "I guess third-degree burns are your thing?";
        ],
    has light;

Bed room_24_bed "patient bed" room_24
    with name 'patient' 'bed',
        description "It's a standard hospital bed. ";

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room station_a "Ward A - Station"
    with description "This is the western end of a long red east-west corridor. Open doorways to the north and south 
        lead to patient rooms, signs next to the former read ~21-A~, the latter ~22-A~. The nurses' station is here. 
        It's a large square waist-high desk used for charting and note-taking. ",
        cheap_scenery
        'south' 'sign' "It reads ~22-A~"
        'north' 'sign' "It reads ~21-A~",
        e_to a_ward_1,
        n_to room_21,
        s_to room_22,
    has light;

Object ward_a_station "nurses' station" station_a
    with name 'nurse' 'station' 'desk',
        description "It's a tall standing desk, square and roughly waist high. It gives the nurses and physicians 
        a surface to update notes and charting. ",
    has supporter scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_21 "Room 21"
    with description "This is an unassuming patient room. The wallpaper is an unsoothing red and a bed is pushed 
        head-first against the wall. A cast-iron radiator drips and hisses under the window. A doorway 
        leads south back out to the hallway. ",
        s_to station_a,
        cheap_scenery
        4 'cast' 'iron' 'cast-iron' 'radiator' [;
            examine:
            "It's a cast-iron steam radiator. It's hissing and knocking in a vain struggle against the chill. ";
            take, touch, pull, push:
            "I guess third-degree burns are your thing?";
        ],
    has light;

Bed room_21_bed "patient bed" room_21
    with name 'patient' 'bed',
        description "It's a standard hospital bed. ";

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_22 "Room 22"
    with description "This is an unassuming patient room. The wallpaper is an unsoothing red and a bed is pushed 
        head-first against the wall. A cast-iron radiator drips and hisses under the window. A doorway 
        leads north back out to the hallway. ",
        n_to station_a,
        cheap_scenery
        4 'cast' 'iron' 'cast-iron' 'radiator' [;
            examine:
            "It's a cast-iron steam radiator. It's hissing and knocking in a vain struggle against the chill. ";
            take, touch, pull, push:
            "I guess third-degree burns are your thing?";
        ],
    has light;

Bed room_22_bed "patient bed" room_22
    with name 'patient' 'bed',
        description "It's a standard hospital bed. ";

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_3_3 "hallway_3_3"
    with description "Here the hallway continues east and west. There's a door to the north. A sign next to it reads ~Conference~. 
        A sign next to to an open doorway to the south reads ~Employees Only~. ",
        w_to elevator_lobby_3,
        n_to conference_door,
        s_to break_room,
        e_to hallway_3_4,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor conference_door "door"
     with name 'north' 'door',
        description [;
            print"It's an unassuming door, currently ";open_or_closed(self);".";
        ],
        door_to [;
            if (parent(self) == hallway_3_3) return conference_room; return hallway_3_3;
        ],
        door_dir [;
            if (parent(self) == hallway_3_3) return n_to; return s_to;
        ],
        found_in hallway_3_3 conference_room,
    has scenery door openable ~open; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room conference_room "Conference Room"
    with description "This is a quiet carpeted room, typically used for family conferences or for delivering bad news. 
        An overstuffed chair is pushed again the wall and a fish tank sits on a wooden pedestal. Everything is beige and very 
        soothing. ",
        s_to conference_door,
    has light;

Object fish_tank "aquarium" conference_room
    with article "an",
        description"It's a large fish tank perched on a wooden pedestal. It's occupied by a handful of 
        goldfish. ",
        before [;
            receive:
            "The fish kindly request that you refrain from trashing their tank. ";
        ],
    parse_name [w1 w2;
        w1 = NextWord();
        w2 = NextWord();
        if ((w1 == 'fish' or 'goldfish') && w2 == 'tank') return 2;
        if (w1 == 'tank' or 'aquarium') return 1;
    ],
    has scenery container transparent open;

Object goldfish "goldfish" fish_tank
    with name 'fish' 'goldfish' 'fishes',
    description "You see a half-dozen goldfish slowly swimming in the tank. ",
    article "a half-dozen",
    before [;
        take:
        "You once had a goldfish named Petey but you overfed him and found him floating belly-up
        after school one day. So, maybe leave the fish alone. ";
        ],
    has scenery;

InChair conference_chair "overstuffed chair" conference_room
    with name 'overstuffed' 'chair',
        article "an",
        description "It's a beige overstuffed chair. ",
    before [;
        take:
            "It's way too heavy. ";
    ],
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_3_4 "hallway_3_4"
    with description "This is the east end of a long east-west corridor. The linoleum-
    lined hallway returns west under yellow fluorescent light. Doors leads north and south. A sign on the 
    former reads 'Womens' Locker'. A sign on the latter reads 'Men's Locker'.",
        before [;
            go:
            if (selected_direction == s_to) "You can't go in the mens' locker room. ";
            if (selected_direction == n_to) print"You push through the door and it swings closed behind you.^^";
        ],
        w_to hallway_3_3,
        n_to storage_door,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor storage_door "swinging door"
     with name 'swinging' 'metal' 'door',
        description [;
            "It's a swinging metal door. ";
        ],
        !parse_name [ w1 w2 ;
        !    w1 = NextWord();
        !    if (w1 == 'swinging') return 1;
        !    if (w1 == 'swinging' or 'metal' && w2 == 'door') return 2;
        !    if (w1 == 'swinging' && w2 == 'door') return 2;
        !],
        door_to [;
            if (parent(self) == hallway_3_4) return storage; return hallway_3_4;
        ],
        door_dir [;
            if (parent(self) == hallway_3_4) return n_to; return s_to;
        ],
        found_in hallway_3_4 storage,
    has scenery door openable open; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room storage "Storage"
    with description [; 
        print"This is a cluttered storage room. The walls are lined with shelves stocked with 
        medical supplies and miscellaneous items that you don't need to concern yourself with. 
        A tall but narrow brown metal cabinet stands in one corner. ";
        if (player notin storage_locker) print"A battered full-length metal locker faces it on the 
        other side of the room. ";
        "A black telephone is mounted on the wall. The exit lies through a metal door to the south. ";
    ],
        before [;
            go:
            if (selected_direction == s_to && player in storage_locker)
            {
                print"(first leaving the locker)^";
                PlayerTo(storage, 1);
                <<go FAKE_S_OBJ>>;
            }
        ],
        s_to hallway_3_4,
    has light;

Object storage_cabinet "cabinet" storage
    with 
        name 'cabinet' 'brown' 'metal' 'tall' 'narrow',
        description [ ;
            print"It's a tall brown metal cabinet, currently ";
            if(self has open) print"open. "; else print"closed. ";
            "A scratched metal handle is embedded in the door. ";
        ],
        before [;
            if (player in storage_locker) "You can't do that from inside the locker. ";
            open:
            if (self has open) "It's already open. ";
            "The cabinet door won't open. It seems to be jammed. ";
            knock:
            <<knock cabinet_door>>;
        ],
    has scenery container openable locked ~open;

Object cabinet_door "cabinet door" storage
    with
        description"It's a metal door with a handle. ", 
        parse_name [ w1 w2 ;
            w1 = NextWord();
            w2 = NextWord();
            if((w1 == 'cabinet' or 'metal') && w2 == 'door') return 2;
            if (w1 == 'door' && w2 == 0) return 1;
        ],
        before [;
            open: 
            <<open storage_cabinet>>;
            close:
            <<close storage_cabinet>>;
            knock:
            if (FlagIsSet(F_SAW_KNOCK_SPOT)) 
            {
                give storage_cabinet open;
                "With a fist you hit the spot that you saw Nurse Retch hit. The cabinet door pops open. ";
            }
            "You thump on the door but nothing seems to happen. ";
        ],
    has scenery;

Object storage_handle "handle" storage 
    with 
        name 'handle',
        description "It's a metal handle, embedded in the cabinet door. It rotates up and down if pulled or 
        pushed. It looks old and cantenkerous. ",
        before [;
            pull:
            if (FlagIsClear(F_CABINET_UNLOCKED)) "You pull with all your might but the cabinet door won't open. It seems to be jammed. ";
        ],
    has scenery locked;

Object storage_locker "locker" storage
    with
        name 'locker' 'metal',
        description"It's a battered full-length locker. The door is ajar and dented. It looks like it won't close all the way. ",
        inside_description"You're crouched in a battered metal locker peering out through a crack in the door. ",
        after [;
            enter:
            "You crouch in the locker and pull the door nearly closed. Through a crack in the door you can see out into 
            the storage room. ";
        ],
    has container open enterable scenery;

Object telephone "telephone" storage
    with name 'black' 'phone' 'telephone',
        description [;
            print"It's a black phone. The base is mounted to the wall and the rotary dial is embedded in the handset. ";
            if (handset in self) "The handset is in the cradle. "; else "";
            ],
        before [;
            DialObj:
                <<dialobj handset>>;
            Take:
                <<take handset>>;
            Drop:
                <<drop handset>>;
        ],  
        describe [;
            rtrue;
        ],
    has scenery container open transparent;

Object handset "handset" telephone 
    with name 'handset',
        description "It's a black phone handset. It has a rotary dial and is attached to the wall unit by a black coiled cord. ",
        mass 0,
        before [;
            DialObj:
                if (self notin player) print "(first taking the handset)^";
                DialPhone();
                move self to telephone;
                "^You hang up the phone. "; 
            Drop:
                if (self in player) {
                    move self to telephone;
                    "You hang up the phone. ";
                }   else "You're not holding the handset. "; 
        ],
        after [;
            Take:
                "You hold it to your ear and hear a dial tone. ";
        ],
        describe [;
            rtrue;
        ],
        react_before [;
            Go:
                if (self in player)   {
                    move self to telephone;
                    print"(first hanging up the phone)^";
                };
            DialNumber:
                if (self notin player) print "(first taking the handset)^";
                DialPhone();
                move self to telephone;
                "^You hang up the phone. ";  
        ],
    has concealed;

Object dial "dial" storage
    with name 'rotary' 'dial',
        description "It's a plastic rotary phone dial. ",
        before [;
            Take:
                "It's part of the handset. ";
        ],
    has scenery;

Object phone_cord "phone cord" storage
    with name 'black' 'coiled' 'cord',
        description "It's a black coil of phone cord. ",
    before [;
            Take:
                "That's attached to the phone.";
    ],
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room break_room "Break Room"
    with 
        description "This is an employee break room. There's a white plastic table here with matching chairs and a 
        refrigerator stands in the corner next to a counter. A small microwave oven occupies the countertop. ",
        n_to hallway_3_3,
    has light;

Object break_room_table "table" break_room
    with name 'white' 'plastic' 'table',
        description "It's a white plastic table. ",
        before [;
            take:
                "You don't need the cheaply-made plastic table. ";
        ],
    has supporter scenery;

Object break_room_chairs "chairs" break_room
    with name 'white' 'plastic' 'chairs//p' 'chair',
        description "You see a number of cheaply-made white plastic chairs. ",
        before [;
            take:
                "You don't need one of those. ";
            enter:
                "You notice the food stains and decide to remain standing. ";
        ],
        has scenery;

Object counter "counter" break_room 
    with name 'counter',
        description "It's a standard formica counter. It's adorned with years of stains and scorch marks. ",
        before [;
            take:
                "Nope. ";
        ],
        has scenery supporter;

Object refrigerator "refrigerator" break_room 
    with name 'refrigerator' 'fridge',
        description "It's an old off-white refrigerator. A helpful handwritten sign taped to the front helpfully reminds the reader 
            that their mother doesn't work here and that one should only eat ones own food. ",
        before [;
            take:
                "You don't possess the power of levitation. You do seem to possess the power of delusion. ";
        ],
    has scenery container openable ~open;

Object fridge_sign "refrigerator sign" break_room
    with name 'sign' 'handwritten' 'note',
        description "It's a handwritten note outlining standard breakroom etiquette. ",
        before [;
            read:
                "~Your mother doesn't work here so CLEAN UP AFTER YOURSELF! Label your food and ONLY EAT WHAT YOU BRING!!~ The 
                abundance of exclamation marks and upper-case letters nearly give you a headache. ";
            take:
                "Then how would people know that their mother doesn't work here?";
        ],
    has scenery;

Object fridge_stuff "smelly collection of tupperware and condiments" refrigerator
    with name 'smelly' 'collection' 'tupperware' 'condiments',
        description "It's an assortment of plastic containers each one labelled with the owner's name. You can also see various bottles and 
            containers of out-of-date condiments. ",
        before [;
            take:
                    "None of it is yours. Didn't you read the sign?";
            smell:
                    "You smell a complex melange of old food and baking soda. ";
        ],
    has static;

Object microwave "microwave oven" break_room
    with name 'microwave' 'oven',
        description [;
                print "It's a small underpowered microwave oven, ";
                if (self.oven_running == true) print "currently running. "; else print"currently off. ";
                "The only control is a dial on the front pointing to 1-minute intervals up to 10 minutes. ";
        ],
        before [;
            take: 
                "It's not a large microwave but it's still not portable. "; 
            open:
                self.oven_running = false;
                StopTimer(microwave_dial);    
        ],
        after [;
            close:
                if (dial_setting == 0) { "You close the microwave. ";
                }   else {
                    StartTimer(microwave_dial, dial_setting);
                    self.oven_running = true;
                    "The microwave starts up again. ";
                }
        ],
        oven_running false,
    has openable container scenery ~open;

Object microwave_dial "dial" break_room
    with name 'dial' 'control',
        description [;
                print"It's a dial that turns in 1-minute increments up to 10 minutes. It's currently pointing to "; 
                if (microwave.oven_running == true) print self.time_left; else print dial_setting;
                ".";
        ],
        before [;
            turnto:
                if(microwave has open) "You should close the microwave first. ";
                if (microwave.oven_running && second == 0) {
                    StopTimer(self);
                    microwave.oven_running = false;
                    dial_setting = 0;
                    "With a 'ding' the microwave shuts off. ";
                }   else {
                    if (second < 0 || second > 10) "It only turns in 1-minute increments up to 10. ";
                    dial_setting = second;
                    StartTimer(self, second);
                    microwave.oven_running = true;
                    "You turn the dial to ", second, ". The microwave turns on with a mechanical hum. ";
                }
        ],
        time_left,
        time_out [;
            microwave.oven_running = false;
            dial_setting = 0;
            "With a 'ding' the microwave shuts off. ";
        ],
        react_before [;
            open:
                if (noun == microwave) {
                    dial_setting = self.time_left;
                }
        ],
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object sub_basement_02 "Sub-Basement"
    with description "This is the eastern end of a short dimly lit hallway that continues to the west.  
        Deep within the bowels of the building, the floor and walls here are dark gray cement and fluorescent
        tubes flicker and buzz annoyingly from above. A service elevator lies to the north.",
    n_to service_elevator_door,
    in_to service_elevator_door,
    w_to sub_basement_01,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object sub_basement_01 "Hallway" 
    with description "This is the western end of a depressingly dim gray hallway. It continues to the east and a
        battered metal door lies to the west. There's a red sign on the door, nearly rusted past 
        legibility. It reads ~Caution: Steam~. An old dumbwaiter is embedded in the wall to the south. ",
        after [;
            go:
            if (selected_direction == e_to && vic notin sub_basement_02) move vic to sub_basement_02;
        ],
    e_to sub_basement_02,
    w_to boiler_door,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor boiler_door "boiler room door"
     with name 'boiler' 'door',
        description [;
            print"It's a heavy thick metal door, currently ";open_or_closed(self);".";
        ],
        door_to [;
            if (parent(self) == sub_basement_01) return boiler_room; return sub_basement_01;
        ],
        door_dir [;
            if (parent(self) == sub_basement_01) return w_to; return e_to;
        ],
        found_in sub_basement_01 boiler_room,
    has scenery door openable ~open; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object boiler_room "Boiler Room" 
    with description "You're standing in a high arched stone and cement-lined space populated buy massive black 
    boilers, squatting against the walls like sleeping dinosaurs. They're all closed and covered with thick coats of dust. 
    A half-dozen metal lamps hang down from the ceiling above, although you can't actually make out the ceiling in the gloom.
    A dented thick metal door leads east. ",
    cheap_scenery
    'boiler' 'boilers' "Each boiler is a black metal hulk squatting on the cement and stone floor. ",
    e_to boiler_door,
    has light;  

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object ceiling_01 "In The Ceiling"
    with description [;
        print"You are perched in the dark service space within the ceiling. Your flashlight reflects off 
        dust motes as you support yourself on thick pipes and brackets suspended from higher up. Large 
        air ducts block any movement to the east but the way to the west is open. Here 
        there is a gap in the drop-ceiling tiles below you and you can see a ladder down below.^";
        ],
        before [;
            go:
            if (selected_direction == d_to && ladder in hallway_m2) 
                if (ladder hasnt open) { "The ladder is folded up. "; }
                else
                {
                print"You gingerly lower yourself through the gap in the drop ceiling, holding onto 
                piping in the ceiling for support until you feel your feet land on the top of the aluminum 
                ladder. Balancing carefully, you finally climb down, relieved to be back on solid ground.^^";
                PlayerTo(hallway_m2);
                rtrue;
                }
            examine:
            if (selected_direction == d_to) "You notice one of the large tiles in the drop-ceiling here out of alignment. 
                Through the gap you can see the hallway and a ladder below. ";
        ],
        w_to ceiling_02,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object ceiling_02 "In The Ceiling"
    with description "You are crouched in the ceiling, separated from the hallway below by tiling. It's dark, dusty, and 
        claustrophic up here. You can crawl east or west along thick pipes and conduits. ",
        e_to ceiling_01,
        w_to ceiling_03;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object ceiling_03 "In The Ceiling"
    with description "You are perched in the service space above the hallway and drop ceiling. Here, it makes a nintey-degree 
        turn and continues to the east and south. ",
        e_to ceiling_02,
        s_to ceiling_04;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object ceiling_04 "In The Ceiling"
    with description "You are crawling in the close and dusty crawl space above the drop ceiling. The piping and 
    brackets supporting you continue to the north and south. ",
        n_to ceiling_03,
        s_to ceiling_05;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object ceiling_05 "In The Ceiling"
    with description [;
        print"You are perched in the dark service space within the ceiling. The passage dead-ends here, 
        and movement to the south is blocked by a large duct. You can crawl north along a network of 
        pipes and brackets into darkness. A tile in the drop-ceiling below is askew, allowing you to look 
        down into an office below. Directly underneath you lies a tall file cabinet that you could probably
        lower yourself down onto.^";
        if (northrup in northrup_office) "^Below you, Dr. Walt Northrup is seated at a large mahogany desk. ";
        ],
        before [;
            go:
            if (selected_direction == d_to) 
            {
                print"Grunting and dust-covered, you lower yourself from the pipes through the drop ceiling, 
                stepping down onto a tall metal file cabinet standing in the corner of the room.^^";
                PlayerTo(file_cabinet, 1);
                if (northrup in northrup_office) 
                {
                    print"A seething Dr. Walter Northrup is here to meet you, furious in equal parts at the cloud of dust you've
                    brought down onto his carpet as well as your ill-conceived invasion of his office. Unsurprisingly, 
                    your time as a candy-striper is at an end.^";
                    deadflag = 6;
                    rtrue;
                }
                rtrue;
            }
            examine:
            if (selected_direction == d_to) "You notice one of the large tiles in the drop-ceiling here out of alignment. 
                Through the gap you can see an office and a cile cabint below. ";
        ],
        n_to ceiling_04,
    has light;



