Class MyNPC
    with 
        max_capacity 24,
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
        description "He's of average build, about 30, and wearing a dirty laboratory
        coat over jeans and a flannel shirt. He wears thick glasses and a mop of brown hair that hasn't seen a comb today.",
        describe [;  
            print"^Oliver is here. ";
            if(self has encountered) "";
            give self encountered;
            "He's the morgue assistant in charge of putting the deceased on ice until the morning. ";
        ],
        npc_arrived [;
            if(parent(self)== morgue) { StopDaemon(self); self.move_mode = 0; rtrue; }
        ],
    class Mover MyNPC
    has animate proper transparent;   

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object freddy "Freddy" folding_chair
    with name 'freddy' 'security' 'guard',
        description [;
            print"He's in his early 20s. He wears a blue uniform shirt, partially untucked, and dark blue 
            pants. He's thin with long black hair, long nose, and gaunt cheeks pockmarked with acne. ";
            if (FREDDY_ASLEEP == true) "He's leaning back in a folding chair snoring. "; else 
                "He's leaning back in a metal folding chair trying hard to stay awake. He's listening to a boombox sitting 
                next to him on the floor. ";
        ],
        describe [;
            print"Freddy is here. ";
            if (self has encountered) rtrue;
            give self encountered;
            "He's the third-shift security guard who's been tasked with guarding Sid Jorry's office until the police can 
            make it here through the blizzard. ";
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
        description "He's a short squat balding gentleman wearing gray custodian overalls.
        He's wearing a walkman and whistling while he pushes a mop in a mop bucket.",
        npc_wander_delay 5,
        npc_avoid stairwell_b,
        npc_walk [; print"pushes the mop bucket";],
        describe [;  
            if (self hasnt encountered)
            {
                give self encountered;
                "^Eugene, the night-shift custodian, is here pushing a mop bucket. ";
            }
            "^Eugene is here, pushing a mop bucket. ";   
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
        describe [;  
            if(self has encountered) "^Vic is here. ";
            give self encountered;
            "^Lurking here in the gloom you see Vic. A tall gaunt figure wearing an insincere smile and a hospital 
            maintenance uniform that doesn't appear to be his. ";
        ],
        description "He's a tall and thin with slicked-back black hair. His eyes are cold and blue and his cheeks are 
            clean-shaven and sunken. ",  
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
    with name 'northrup' 'doctor' 'walt' 'walter',  
        describe [;  
            if(self has encountered) "^Dr. Northrup is here. ";
            give self encountered;
            print"^Dr. Walter Northrup is here, sitting at his desk.^";
        ],
        description "He's a tall silver-haired gentleman with a patrician bearing. He's wearing a perfectly-creased 
        white lab coat over a crisp shirt and tie. ",
    class Mover MyNPC
    has animate proper;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object mabel "Mabel" main_lobby
    with name 'mabel',
        description "She's an elderly African American lady. From previous conversations with her you know that she's 82,
        but her unlined face and white shock of curly hair make her look much younger. She wears a floral print dress with numerous 
        layers over it and wire bifocals on a chain. ",
        describe [;  
            if(self has encountered) "^Mabel is here behind the information desk. ";
            give self encountered;
            print_ret"^Mabel is here behind the information desk. She sees you and waves. ~Hey there Candy girl. 
            Got any new books for me?~";
        ],
    class Mover MyNPC
    has animate female proper;
