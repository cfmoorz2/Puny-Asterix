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
            12 'white' 'tile' 'tiles//p' "The tiles are cracked and there's some greenish gunk growing in the spaces between them. "
            1 'drain' "It's a standard drain in the floor about three inches across. "
            21 'flickering' 'fluorescent' 'tubes//p' "They're standard fluorescent lights. They emit a sickly green light that 
                makes everyone look corpse-like. "
            12 'refrigerated' 'locker' 'lockers//p' [;
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
        !open_or_closed(elevator_doors);
        ". The call button is embedded in a small panel next to the door.";
        ],
        e_to basement_hallway_east,
        n_to stairwell_b,
        !s_to elevator_doors,
        !in_to elevator_doors,
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
        !n_to service_elevator_door,
        !in_to service_elevator_door,
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
    with description [;
        print"This is the western end of a long corridor that stretches to the east under flickering fluorescent lights. 
        There's an open doorway to the north, a sign next to it reads ~Environmental Services~.
        A second doorway lies to the west. A sign next to it reads ~Engineering~. Finally, a substantial metal door lies to the 
        south, a placard on it reads ~Central Supply.~ There's a magnetic key card swipe next to it, a small light on it is ";
        if (card_reader has on) "green."; else "red.";
        ],
        after [;
            Go:
                if (selected_direction == n_to) {
                    give central_supply_door ~open;
                    give central_supply_door locked;
                    give card_reader ~on;
                    "As you pass through the door, it closes behind you with a 'click'. The light on the card reader turns red. ";
                }
        ],
        e_to hallway_b1,
        n_to environmental_services,
        s_to central_supply_door,
        w_to engineering,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room central_supply_anteroom "Anteroom" 
    with description "This is a dark utility basement. The floor and walls are cement and pipes run along the ceiling. 
        This is a small anteroom just off of the larger space. A metal door lies to the north and a passage turns to the west, 
        deeper into the darkness.",
        n_to central_supply_door,
        after [;
            Go:
                if (selected_direction == s_to) {
                    give central_supply_door ~open;
                    give central_supply_door ~locked;
                    "As you pass through the door, it closes behind you with a 'click'.";
                }
        ],
        before [;
            Go:
                if (selected_direction == n_to) give central_supply_door open;
        ],
        w_to central_supply,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room central_supply "Central Supply" 
    with description "This is a large basement filled with dusty crates and defunct antique equipment. There are scattered low-wattage lamps 
    hanging from the ceiling but most of these have long ago burned out, leaving the room quite dark with grotesque shadows 
    projected onto the cement walls. An impossible tangle of pipes and conduits hangs from above. A black phone is mounted
    to the wall here. ",
    e_to central_supply_anteroom,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 myDoor central_supply_door "central supply door" 
    with name 'central' 'supply' 'door',
        description [;
            print"It's a thick metal door, currently ";
            open_or_closed(self);
            print ".";
            if (real_location == hallway_b2) {
                print "There's a magnetic swipe card reader next to it. The little light on it is ";
                if (card_reader has on) "green."; else "red.";
            }
        ],
        door_to [;
            if (parent(self) == hallway_b2) return central_supply_anteroom; return hallway_b2;
        ],
        npc_open [npc;
            give self ~locked;
            give self open;
            if (real_location == hallway_b2) print_ret(The)npc," swipes a card through the reader and passes through 
            the security door. ";
            if (real_location == central_supply_anteroom) print_ret(The)npc," opens the security door and passess through. ";
        ],
        door_dir [;
            if (parent(self) == hallway_b2) return s_to; return n_to;
        ],
        found_in central_supply_anteroom hallway_b2,
    has scenery door openable ~open locked;

Object card_reader "card reader" hallway_b2
    with name 'reader' 'card',
        description [;
            print "It's a magnetic swipe card reader built into the side of the door. The little light is ";
            if (self has on) "green. "; else "red. ";
        ],
        time_left,
        time_out [;
            give card_reader ~on;
            give central_supply_door ~open;
            if(real_location ~= central_supply or central_supply_anteroom) give central_supply_door locked;
            if (TestScope (card_reader, player)) "You hear a 'click' from the door and the light on the card reader turns red. ";
        ],
    has scenery;

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
        !s_to elevator_lobby_2,
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
        !open_or_closed(elevator_doors);
        ". A stairwell lies to the north through an open doorway. The hallway leads east and west. On the wall here you 
        see ~Ward B~ posted in large black letters above an arrow pointing west and a blue line on the floor starts here 
        and leads in that direction as well. ";
    ],
    n_to stairwell_3,
    !e_to hallway_3_3,
    !s_to elevator_doors,
    !in_to elevator_doors,
    w_to hallway_3_1,
    has light; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room hallway_3_1 "hallway_3_1"
    with description "Here the hallway continues east and west. A service elevator is here in the north wall. 
        A blue line is painted down the middle of the floor. ", 
        e_to elevator_lobby_3,
        w_to hallway_3_2,
        !n_to service_elevator_door,
        !in_to service_elevator_door,
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
        !n_to room_33,
        s_to room_34,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_34 "Room 34"
    with description "This is a single-occupancy patient room. The walls are blue and a single bed is pushed against the wall. ",
        n_to b_ward_1,
    has light;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Room room_32 "Room 32"
    with description "This is a single-occupancy room. The walls are blue and a single bed is pushed against the wall.
        The floor is scuffed linoleum and a steaming radiator hisses under a window. ",
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
    !n_to room_31,
    s_to room_32,
    has light;

Object ward_b_station "nurses' station" station_b
    with name 'nurse' 'station' 'desk',
        description "It's a tall standing desk, square and roughly waist high. It gives the nurses and physicians 
        a surface to update notes and charting. ",
        has supporter scenery;

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
Room elevator_lobby_m "Main Elevator Lobby"
    with description [;
        print"This is the main elevator lobby. The elevator doors, currently ";
        !open_or_closed(elevator_doors); 
        ", lie to the south, the 'up' and 'down' buttons 
        are embedded in a small panel next to them. An open doorway leads into a stairwell to the north and an open double 
        doorway leads to the east. A sign across the top reads ~Cafeteria~. A long corridor begins here and leads west. ";
        ],
        cheap_scenery
            'cafeteria' 'sign' "It's a large sign over the doorway that reads ~Cafeteria~. ",
        n_to stairwell_m,
        e_to cafeteria,
        w_to hallway_m1,
        !s_to elevator_doors,
        !in_to elevator_doors,
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
            23 'steam' 'serving' 'table' 'buffet' 'station' [;
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
        22 'steel' 'prep' 'tables//p' 'table' [;
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
        12 'fluorescent' 'light' 'lights//p' [;
            take:
            "You can't reach them and don't need them. ";
            examine:
            "They're bare fluorescent tubes. They're all off. ";
        ]
        12 'exit' 'light' 'sign' [;
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
        s_to lobby_west,
        !w_to hallway_m2,
        e_to elevator_lobby_m,
        !n_to service_elevator_door,
        !in_to service_elevator_door,
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
    !w_to security_door,
    has light;

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
                    "You push through the doors and with a hiss they close behind you.^";
                }
        ],
        cheap_scenery
        11 'information' 'desk' "It's a round wooden desk with the word 'Information' emblazoned across the front. "
        12 'lobby' 'windows//p' 'window' "They're icing over and difficult to see through. Through them you can 
            intermittently catch a glimpse of the blizzard outside. ",
        w_to lobby_west,
        s_to main_lobby_doors,
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
