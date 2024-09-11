Room morgue "Morgue"
    with name 'morgue',
        description [;
                print "This is the hospital's small morgue. The white tiled floor is scuffed and gently slopes toward a drain in the center of the
                room. The walls are also clad in featureless tiling and a pair of flickering fluorescent tubes above you impart a sickly green 
                ambiance. A stainless steel table sits in the center of the room and one wall is occupied by a bank of refrigerated
                lockers. A door, currently ";
                open_or_closed(morgue_door);
                print" lies to the west.^^A body is lying on the table";
                if(sheet in body) ", currently covered by a sheet. "; else ".";
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
            examine:
                if (sheet in self) "It's currently covered by a sheet. ";
            take:
            "He'd just be dead weight. ";
        ],
    has scenery supporter;

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
        u_to 0,
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