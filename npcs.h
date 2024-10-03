Class MyNPC
    with 
        holding [counter i;
            objectloop(i in self && i hasnt worn)   {
                !counter = counter + i.mass;
            }
            return counter;
        ],
        npc_post_follow [; rfalse;],
        react_before [ x;
            take:
            if (noun has scenery || noun has static) rfalse;
            x = self.holding();
            !if (x + noun.mass > self.max_capacity) "That's more than ",(name)self," can hold.";
            go:
            if (following) { following = false; rfalse; }
            if(player == self && real_location provides noun.door_dir && real_location.(noun.door_dir) ofclass Room)
            {
                if(noun == u_obj) { print"You head up the stairs.^"; rfalse; }
                if(noun == d_obj) { print"You head down the stairs.^"; rfalse; }
                if(noun == in_obj) { print"You head inside.^"; rfalse; }
                if(noun == out_obj) { print"You head out.^"; rfalse; }
                print"You head off to the ",(name)noun,".^";
            }
        ],
        npc_after_action    [ i;
            objectloop (i in parent(self))  {
                if(i has valuable) {
                    move i to self;
                    if(TestScope(self)) print(The)self," picks up ",(a)i,".^";
                }
            }
            rfalse;
        ];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object oliver "Oliver" morgue
    with name 'oliver' 'mortuary' 'assistant',
        max_capacity 34,
        description "He's the morgue assistant. He's of average build, about 30, and wearing a dirty laboratory
        coat over jeans and a flannel shirt. He wears thick glasses and a mop of brown hair that hasn't seen a comb today.",
        npc_arrived [;
            if(parent(self)== morgue) { StopDaemon(self); self.move_mode = 0; rtrue; }
        ],
        describe [;
            if(self in morgue) "^Oliver the morgue assistant is here doing some paperwork. ";
        ],
    class Mover MyNPC
    has animate proper transparent;   

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object freddy "Freddy" folding_chair
    with name 'freddy' 'security' 'guard',
        description [;
            print"He's the third-shift security guard who's been tasked with guarding Sid Jorry's office until the police can 
            make it here through the blizzard. He's in his early 20s. He wears a blue uniform shirt, partially untucked, and dark blue 
            pants. He's thin with long black hair, long nose, and gaunt cheeks pockmarked with acne. ";
            if (FREDDY_ASLEEP == true) "He's leaning back in a folding chair snoring and deeply asleep. "; else 
            "He's leading back against the wall in a metal folding chair. His lids are heavy and
            he's having a tough time staying awake. ";
        ],
    class Mover MyNPC
    has animate proper transparent;   

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object buzz "Buzz" security_chair
    with name "Buzz",
        take_shrimp false,
        description "He's an obese middle-aged gentleman in his fifties. His hair is close-cropped and he's wearing a stained dark blue 
        security uniform. ",
        daemon [;
            if (self.take_shrimp && TestScope(self)) {
                self.take_shrimp = false;
                print"^Buzz plucks a shrimp from the bowl and plops it into his mouth.^";
                if(shrimp in shrimp_bowl)   {
                    print"^Suddenly, he freezes and beads of sweat break out on his expansive forehead. A loud wet gurgling sound 
                    emanates from his large stomach. Frantically, he bolts up and with shocking dexterity, dashes out of the room 
                    and disappears.^";
                    remove buzz;
                    remove shrimp;
                    StopDaemon(buzz);
                }
            } else {
                self.take_shrimp = true;
            }
        ],
        life [;
            give, show:
            if (noun == shrimp) 
            {
                move shrimp to shrimp_bowl;
                "Buzz's eyes widen at the sight of the shrimp.^
                ~Ooh, thought you could get away, huh?~^
                Before you can react, he plops the shrimp in the bowl with the others. ";
            } 
            "Buzz doesn't seem interested in the ",(name)noun,".";
        ],
    has animate proper transparent;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object sid_jorry "Sid Jorry" jorry_chair
    with name 'sid' 'jorry',
        description [;
            print"He's a corpulent gentleman with a bushy mustache and a circle of black hair wrapping around
            his otherwise bald head. He always reminded you of your uncle Pete except he's wearing a gray business 
            suit and Uncle Pete only wears black socks and cargo shorts. Jorry is seated at his desk and slumped
            forward across it. Luckily, his eyes are closed because that would be too creepy otherwise. ";
            if (band_aid in self) "You notice a band_aid stuck to the right side of his neck. "; 
                else "You notice a tiny puncure wound on the right side of his neck. ";
        ],
        before [;
            push, pull:
            "You're too creeped out to mess with him. ";
            search:
            "You force yourself to go through his pockets but find nothing of interest. ";
            wakeother:
            "He's dead, not sleeping. ";
            take:
            "He would just be dead weight. ";
        ],
        life [;
            give, show:
            "Unsurprisingly, he doesn't seem interested in ",(the)noun,".";
            ask:
            "I wouldn't wait around for an answer. ";
            tell:
            "I don't think he's going to respond. ";
        ],
    has scenery animate transparent;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object eugene "Eugene" environmental_services
    with name 'eugene' 'gene' 'custodian' 'janitor',
        description "Eugene is the night-shift custodian. He's a short squat balding gentleman wearing gray custodian overalls.
        He's wearing a walkman and whistling while he pushes a mop in a mop bucket.",
        npc_wander_delay 5,
        npc_avoid stairwell_b,
        npc_walk [; print"pushes the mop bucket";],
        describe [;
            "^Eugene the janitor is here, bopping and mopping. ";
        ],
    class Mover MyNPC
    has animate proper transparent;  

Object mop "mop" eugene
    with name 'wet' 'mop',
        description "It's a standard wet mop. It's sitting in the mop bucket. ",
        before [;
            take:
            "You reconsider. Ew, that mop water is SO gross. ";
        ],
    has scenery;

Object bucket "bucket" eugene 
    with name 'bucket',
        description "It's a wheeled metal bucket full of brownish water with one of those wringer thingies on the side. ",
        before [;
            take:
            "Ooh. No. It's heavy and full of smelly water. ";
            receive:
            "The bucket is already full of brackish smelly water. ";
        ],
    has scenery container open;

Object water "brown water" bucket
    with name 'brown' 'smelly' 'water',
        description "It's a bucketfull of smelly brown water. Yuck. ",
        before [;
            take:
            "Oh, no way. ";
            drink:
            "As if...";
            smell:
            "Oh my god, it smells like barf. ";
        ],
    has scenery;

Object wringer "wringer" bucket 
    with name 'wringer',
        description "It's a metal squeezy-thing with a handle hanging on the side of the bucket. ",
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object vic "Vic"
    with name 'vic',
        description [;
            if (FlagIsSet(F_BADDIES_FOLLOWING)) "He's smiling and seems to be enjoying himself. His right hand is 
            ominously thrust into a pocket."; 
            "He's a tall and thin with slicked-back black hair. His eyes are cold and blue and his cheeks are 
            clean-shaven and sunken. He's wearing an ill-fitting hospital maintenance uniform that doesn't appear 
            to be his. ";
        ],
        each_turn [;
            if(real_location == sub_basement_02)
            {
                if (syringe in player || denim_jacket in player) print"^Vic eyes you over suspiciously. ~Hey there, 
            little lady. Whatcha got there now?~^";
                if (syringe in player && denim_jacket notin player) { print "He plucks the syringe from you.^"; remove syringe; }
                if (denim_jacket in player && syringe notin player) { print "He plucks the jacket from you.^"; remove denim_jacket; }
                if (syringe in player && denim_jacket in player) { print "He plucks the jacket and syringe from you.^"; remove denim_jacket;
                    remove syringe; }  
            }          
        ],     
    class Mover MyNPC
    has animate proper;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object northrup "Dr. Northrup" northrup_office
    with name 'dr' 'northrup' 'doctor' 'walt' 'walter',  
        description [;
        if(FlagIsSet(F_BADDIES_FOLLOWING)) "His carefully coifed hair is disheveled and his patrician manner is betrayed by wide eyes 
            and a red face. ";
        "He's a tall silver-haired gentleman with a patrician bearing. He's wearing a perfectly-creased 
        white lab coat over a crisp shirt and tie. ";
        ],
    class Mover MyNPC
    has animate proper;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object mabel "Mabel" main_lobby
    with name 'mabel',
        description "She's an elderly African American lady. She's 82 but her unlined face and white shock of curly hair 
        make her look much younger. She wears a floral print dress with numerous layers against the chill and wire bifocals on a chain. ",
        talk_array talk_array_mabel,
    class Mover MyNPC
    has animate female proper;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object becky "Becky" station_a
    with name 'nurse' 'becky',
        description "She's the nurse on duty on Ward A tonight. She's a cheerful blonde in 
        her late 20s and St. Asterix is her first nursing job. She's attired in the standard white nursing 
        uniform and is wearing black and white checkered Vans. ",
        describe [; print"^Nurse Becky is here, ";
            print_ret (string) random ("charting some vital signs. ", "adjusting her white nurse's cap. ",
            "idly playing with a sphygmomanometer. ");
        ],
        !talk_array talk_array_becky, 
    class Mover MyNPC
    has animate proper transparent; 

Object sphygmomanometer "sphygmomanometer" becky
    with
        name 'sphygmomanometer' 'cuff',
        description "It's a standard sphygmoma-whozit, used to take blood pressures. ",
        before [;
            take:
            "It's nearly unpronounceable, so you don't need it. ";
        ],
    has scenery; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object linda "Linda" station_a
    with name 'aide' 'linda',
        description "She's the super-cheery nurse's aide assigned to ward A. She's in her 40s and is 
        perpetually smiling. She has bright red hair curled into a tight perm and wears green scrubs. ",
        describe [; print"^Linda the nurse's aide is here, ";
            print_ret (string) random( "twirling a curl of hair with an index finger. ",
            "smiling. ", "humming to herself. ");
        ],
        npc_wander_delay 3,
        npc_avoid hallway_3_2,
        !talk_array talk_array_linda, 
    class Mover MyNPC
    has animate proper transparent; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object maria "Maria" station_b
    with name 'aide' 'maria',
        description "She's the nurse's aide assigned to ward B. She's a latina in her 30s. 
        She has long black hair pulled back into a pony-tail and wears pink scrubs. ",
        describe [; "^Maria the nurse's aide is here, being helpful. "; ],
        !talk_array talk_array_maria, 
        npc_wander_delay 3,
        npc_avoid hallway_3_2,
    class Mover MyNPC
    has animate proper transparent;  

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object nurse_retch "Nurse Retch" station_b
    with name 'nurse' 'retch',
        description [;
            if(FlagIsSet(F_BADDIES_FOLLOWING)) "Her eyes seem to almost be glowing like hateful embers and you can clearly see prominent 
                veins lining her temples. ";
                "She's the nurse assigned to Ward B tonight. She's an austere-looking woman in her 40s. Her jet black hair is pulled back into a 
                tight bun and severe green eyes peer out over high gaunt cheekbones. She wears a white 
                nurse uniform with a white nurse cap pinned to her hair. ";
        ],
        describe [;
            if (self in station_b) "^Nurse Retch is here performing some nursing-related activities. ";
            ],
        life [;
            give, show:
            if (noun == syringe) 
            {
                remove syringe;
                SetFlag(F_RETCH_TRIGGERED);
                StartTimer(retch_timer, 4);
                "^For an instant you could swear you see a flash of fear cross her face. Then, her thin lips
                relax into a saccharine smile. ~Wherever did you find that, dear?~ She takes it from you. ~Can't 
                have you sticking yourself now, can we?~";
            }
            if (noun == kcl_bottle)
            {
                print"She looks at the plastic bottle and a look of pure hate flashes across her face.^^
                ~You are quite the persistent little thing, aren't you?~ She angrily swipes the vial from you.^^
                ~You'd best be careful, candystriper, or you may not finish out your time here.~^";
                remove kcl_bottle;
                SetFlag(F_RETCH_KCL);
                nurse_retch.move_mode = TARGET_PATH;
	            nurse_retch.target_room = northrup_office;
                StartDaemon(nurse_retch);
                rtrue;
            }
        ],
        npc_arrived [;
            if(parent(self)== storage) 
            { 
                StopDaemon(self); 
                self.move_mode = 0; 
                StartTimer(retch_timer_2, 3);
                rtrue; 
            }
        ],
        
        each_turn [;
            if (self in storage && player in storage_locker)
            {
                print"^Through the crack in the locker door you see her move to the metal cabinet in the corner. 
                She nervously looks around then pulls on the door handle. She grunts angrily then pauses and balls up a fist. 
                She bangs on the door in a specific spot and it pops open.^^
                You see her take something from a pocket, put it in the cabinet, and close the cabinet door.^^
                Looking around one last time, she leaves the room to the south.^";
                move self to hallway_3_4;
                move kcl_bottle to storage_cabinet;
                SetFlag(F_SAW_KNOCK_SPOT);
                nurse_retch.hide = 1;
                retch_timer_2.time_out();
            }
        ],
    class Mover MyNPC
    has animate proper transparent;  

Object retch_timer
    with 
        time_left,
        time_out [;
            nurse_retch.move_mode = TARGET_PATH;
	        nurse_retch.target_room = storage;
            StartDaemon(nurse_retch);
        ];

Object retch_timer_2
    with 
        time_left,
        time_out [;
            nurse_retch.move_mode = TARGET_PATH;
            nurse_retch.target_room = station_b;
            StartDaemon(nurse_retch);
        ];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object trio "Retch, Northrup, and Vic" 
    with 
        id 0, ! 1 = northrup, 2 = retch, 3 = vic
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if ((w1 == 'northrup' or 'doctor') && w2 == 0) { self.id = 1; return 1; }
            if (w1 == 'doctor' && w2 == 'northrup') { self.id = 1; return 2; }
            if ((w1 == 'retch' or 'nurse') && w2 == 0) { self.id = 2; return 1; }
            if (w1 == 'nurse' && w2 == 'retch') { self.id = 2; return 2; }
            if (w1 == 'vic' && w2 == 0) { self.id = 3; return 1; }
        ],
        description [;
            switch (self.id)
            {
                1: "His carefully coifed hair is disheveled and his patrician manner is betrayed by wide eyes 
                and a red face. ";
                2: "Her eyes seem to almost be glowing like hateful embers and you can clearly see prominent 
                veins lining her temples. ";
                3: "He's smiling and seems to be enjoying himself. ";
            }
        ],
        npc_arrived [;
            if (parent(self) == mri_scanner)
            {
                StopDaemon(self); 
                self.move_mode = 0;
                rtrue;
            }
        ],
    class Mover MyNPC
    has animate proper pluralname transparent; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object injured_trio "Retch, Northrup, and Vic" 
    with 
        id 0, ! 1 = northrup, 2 = retch, 3 = vic
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if ((w1 == 'northrup' or 'doctor') && w2 == 0) { self.id = 1; return 1; }
            if (w1 == 'doctor' && w2 == 'northrup') { self.id = 1; return 2; }
            if ((w1 == 'retch' or 'nurse') && w2 == 0) { self.id = 2; return 1; }
            if (w1 == 'nurse' && w2 == 'retch') { self.id = 2; return 2; }
            if (w1 == 'vic' && w2 == 0) { self.id = 3; return 1; }
        ],
        description [;
            switch (self.id)
            {
                1: "He's lying on the floor. He's moaning, battered, and bruised. ";
                2: "She's grunting painfully and angrily, rolling around on the floor. ";
                3: "His face is bruised and swollen and he groans painfully. ";
            }
        ],
        describe [;
            "Dr. Northrup, Nurse Retch, and Vic are here, incacapitated on the floor. ";
        ],
    has animate proper pluralname transparent; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object winston "Mr. Winston" hallway_3_4
    with 
        name 'mr' 'winston',
        description "He's a thin enderly gentleman with sparse strands of white hair on a bald head 
        pocked with age spots. A gray four-day stubble shades his cheeks and chin. He's wearing a 
        hospital gown and slip-proof socks. He pushes a wheeled IV pole in front of him. ",
        describe [;  
            print"^Mr. Winston is here. ";
            if(self has encountered) "";
            give self encountered;
            "He's a bored patient pushing an IV pole up and down the wards. ";
        ],
        npc_walk [; print"pushes his IV pole";],
        npc_wander_delay 4,
        npc_avoid storage conference_room break_room stairwell_3 room_32 room_33 room_34,
    class Mover MyNPC
    has animate proper transparent; 

Object winston_pole "IV pole" winston
    with name 'iv' 'i.v.' 'pole',
        description"It's a wheeled metal pole with a hook at the top for hanging IV fluids. There's a clear bag of 
            saline hooked to it. ",
        before [;
            take:
                if (noun == self) "You should leave Winston's IV alone. ";
            ], 
    has static;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object edith "Edith" room_32
    with name 'edith',
        description "She's a thin elderly woman resting on the bed. Her gray hair is messily sprawled across her pillow 
        and she's covered by a threadbare green blanket. ",
        !talk_array talk_array_edith, 
        describe [;
            if (self hasnt encountered) 
            {
                give self encountered;
                "^Edith, a frial elderly patient is lying in the bed. ";
            }
            "^Edith is lying in the bed. ";
        ],
    class Mover MyNPC
    has animate proper transparent;  

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object ms_parsons "Ms. Parsons" room_22
    with name 'ms' 'parsons',
        description "She's a small pouch of wrinkled skin and scant white hair. She's nearly swallowed up
        by her gown and the bed she's lying in. She's smiling but her eyes are closed. You pause for a moment 
        to make sure she's breathing... Yes. She is. ",
        describe [;
            if (self hasnt encountered) 
            {
                give self encountered;
                "^Ms. Parsons, a frail hyper-elderly lady is lying in the bed. She's recognized as the oldest resident of the 
                Tri-state area. The best guess is that she's 103 years old. She has dementia and doesn't talk much but is always 
                smiling. She once claimed that the secret to her longevity was that she never married. ";
            }
            "^Ms. Parsons is lying in the bed. ";
        ],
    class Mover MyNPC
    has animate proper transparent;  

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object larry "Larry" room_33
    with name 'larry',
        description "He's a jaundiced gentleman in his 60s. He's quite large; the hospital bed barely contains him. 
        He has thick grey hair and his cheeks are ruddy. ",
        describe [;
            if (self hasnt encountered) 
            {
                give self encountered;
                "^Larry, a corpulent gentleman is lying in the bed. ";
            }
            "^Larry is lying in the bed. ";
        ],
        !talk_array talk_array_larry, 
    class Mover MyNPC
    has animate proper transparent;  

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object elliot "Elliot" hallway_2_3
    with name 'elliot',
        description "He's a young athletic blonde blond gentleman in his 20s. He's rolling idly up and 
        down the hallways in a wheelchair, his right leg propped in front of him in a cast. ",
        describe [;  
            print"^Elliot is here. ";
            if(self has encountered) "";
            give self encountered;
            "He's rolling up and down the hallway in his wheelchair. ";
        ],
        npc_walk [; print"rolls";],
        npc_wander_delay 3,
        npc_avoid radiology stairwell_2 room_21 room_22 room_23,
    class Mover MyNPC
    has animate proper transparent; 

Object wheelchair "wheelchair" elliot
    with
        name 'wheelchair' 'chair' 'wheels',
        description "It's a standard wheelchair. Elliot is occupying it currently. ",
        before [;
            take:
            "It's currently in use. ";
            push:
            "Elliot waves off your assistance. ";
        ],
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object mrs_chen "Mrs. Chen" room_21
    with 
        name 'mrs' 'chen',
        description "She's a small birdlike cheery Chinese lady with short gray hair and a constant smile 
            on her face. She doesn't speak English but retains an air of wise insight. ",
        describe [;
            if (self hasnt encountered) 
            {
                give self encountered;
                "^Mrs. Chen, a small Chinese lady is lying in the bed. ";
            }
            "^Mrs. Chen is lying in the bed. ";
        ],
    class Mover MyNPC
    has animate proper transparent; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object mr_johnson "Mr. Johnson" room_23
    with name 'mr' 'johnson',
        description "He's a middle-aged gentleman of average height pushing a rolling walker. He's tan with 
        short black hair. He's wearing a gown and non-slip socks. ",
        describe [;  
            print"^Mr. Johnson is here. ";
            if(self has encountered) "";
            give self encountered;
            "He's pushing a rolling walker ahead of him up and down the halls. ";
        ],
        npc_walk [; print"rolls";],
        npc_wander_delay 2,
        npc_avoid stairwell_2 mri_anteroom x_ray room_21 room_22 room_24,
    class Mover MyNPC
    has animate proper transparent; 
