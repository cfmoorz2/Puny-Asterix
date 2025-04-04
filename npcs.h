Class MyNPC
    with 
        holding [counter i;
            objectloop(i in self && i hasnt worn)   {
            }
            return counter;
        ],
        life [;
            ask, tell, order:
            "You can 'talk to ",(name)self,"'.";
        ],
        before [;
            search:
            "Leave the pat-downs to the professionals. ";
        ],
        react_before [ x;
            take:
            if (noun has scenery || noun has static) rfalse;
            x = self.holding();
            go:
            if (following) { following = false; rfalse; }
            if(player == self && real_location provides noun.door_dir && real_location.(noun.door_dir) ofclass Room)
            {
                if(noun == u_obj) { print"You head up the stairs.^"; rfalse; }
                if(noun == d_obj) { print"You head down the stairs.^"; rfalse; }
                print"You head off to the ",(name)noun,".^";
            }
        ],
        signature_desc [;
            if(self has female) print"She "; else print"He ";
            "takes the letter and pen, signs it and hands it back. ";
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
        description "He's the morgue assistant. He's of average build, about 30, and wearing a dirty laboratory
        coat over jeans and a flannel shirt. He wears thick glasses and a mop of brown hair that hasn't seen a comb today.",
        npc_arrived [;
            if(parent(self)== morgue) { StopDaemon(self); self.move_mode = 0; rtrue; }
        ],
        talk_array talk_array_oliver,
        describe [;  
            print"^Oliver is here. ";
            if(self has encountered) "";
            give self encountered;
            "He's the morgue assistant assigned to the night shift. ";
        ],
        life [;
            give, show:
            switch (noun)
            {
                form: add_signature(self); rtrue;
                syringe: "~Huh, that's odd.~";
                kcl_bottle: "~That's strange. That shouldn't be out for you to find.~";
                walkman: "~Yes, you have a walkman.~";
                thriller: "~Yeah, I've heard it, obviously.~";
                air_supply_tape: "~They're a little too sedating for my taste.~";
                shrimp: "~Ugh! Get that out of my face.~";
                ledger: "~That looks like it might be important to someone else.~";
                default: "~Yes, ",(the)noun," is very nice,~ he waves you away impatiently. ";
            }
        ],
        signature_desc [;
            "~Sure, whatever. I'll sign whatever you need. I wish I had a letter that'd get ME out of here.~";
        ],
    class Mover MyNPC
    has animate proper transparent;   

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Array freddy_avoid_array --> admin_hallway hallway_b2 basement_hallway_east cafeteria a_ward_1 b_ward_1;
Object freddy "Freddy" hallway_m2
    with name 'freddy' 'security' 'guard',
        description [;
            print"He's the third-shift security guard. He's in his early 20s and wears a blue uniform shirt, partially untucked, and dark blue 
            pants. He's thin with long black hair, long nose, and gaunt cheeks pockmarked with acne. He's yawning and can barely keep
            his eyes open. ";
            if (FlagIsSet(F_FREDDY_ASLEEP)) "He's standing here, deeply asleep. "; else 
            "He's making his rounds up and down the hallways. ";
        ],
        life [;
            ask: 
            if(second == dull_key) "Dr. Northrup told me to keep the key to myself, dude. ";
            show, give:
            if (FlagIsSet(F_FREDDY_ASLEEP)) "He's currently asleep. ";
            if (noun == walkman)
            {
                if (FlagIsSet(F_FREDDY_WALKMAN_SHY)) "~Whoah, dude, no way. I can't get caught napping. My uncle's already thinking about firing me.~";
                if (FlagIsSet(F_HEADPHONES_ARE_UNPLUGGED)) "He's not interested since the headphones are unplugged. ";
                print"He takes the walkman and headphones. ~Ooh.  Whatcha listenin' to, little dudette?~^^
                He puts the headphones over his ears. ";
                if (walkman has open) { print"He closes the tape compartment. "; give walkman ~open; }
                if (~~walkman_playing) { print "He presses the 'play' button. "; walkman_playing = true; }
                if (air_supply_tape in walkman)
                {
                    SetFlag(F_FREDDY_ASLEEP);
                    move walkman to freddy;
                    give walkman ~worn;
                    scope_modified = true;
                    StopDaemon(freddy);
                    "The tuneful adult contemporary strains of 'Air Supply' wash over the 
                    exhausted security guard. His eyes close and he smiles. In another moment he snores and you 
                    realize that he's fallen dead asleep on his feet. ";
                }
                if (thriller in walkman)
                {
                    walkman_playing = 0;
                    if (FlagIsSet(F_FREDDY_HAS_HEARD_THRILLER)) "~Yeah, I've heard it.~ He presses the 'stop' button and hands your walkman back. ";
                    SetFlag(F_FREDDY_HAS_HEARD_THRILLER);
                    "~Oh, yeah, ~ (he's talking inappropriately loudly with the headphones on). ~My little sister and 
                    my grandmother love this album.~ He presses the 'stop' button and hands your walkman back. ";
                } 
                walkman_playing = 0;
                "He listens for a moment but doesn't seem impressed by your musical tastes. He presses 'stop' and hands the walkman back to you. ";
            }
            switch (noun)
            {
                form: add_signature(self); rtrue;
                thriller: "~My girlfriend plays that thing non-stop.~";
                air_supply_tape: "~That's old person naptime music.~";
                shrimp: "He covers his nose with his arm. ~Aw, man! That's rank!~";
                coaster: "~Dude, they've got great lasagna. And if you ever need a handgun you can usually find one in one of the men's room stalls.~";
                default: "He yawns and seems uninterested. ";
            }
            wakeother:
            if (FlagIsClear(F_FREDDY_ASLEEP)) "He's already awake. ";
            "He's too deep in the gauzy pop music. ";
        ],
        signature_desc [;
            "~What? You don't work here, dude? Sure I suppose I can sign your petition.~";
        ],
        talk_array talk_array_freddy,
        before [;
            talk:
            if (FlagIsSet(F_FREDDY_ASLEEP)) "He's currently asleep. ";
            push:
            if (FlagIsSet(F_FREDDY_ASLEEP)) "Don't tip a sleeping Freddy. ";
            search:
            if (FlagIsSet(F_FREDDY_ASLEEP))
            {
                move dull_key to real_location;
                "You awkwardly search the snoozing security guard. While mucking around in his pockets, a key 
                falls out and hits the floor with a ~clink~. ";
            }
        ],
        describe [;  
            print"^Freddy is here. ";
            if (FlagIsSet(F_FREDDY_ASLEEP)) "He's literally asleep on his feet. ";
            if(self has encountered) "";
            give self encountered;
            ActivateTopic(buzz, 301);
            ActivateTopic(becky, 309);
            "He's the third-shift security guard. He's also the drummer in a local band, The Roomy Sedans, and spends his days 
            rehearsing in his cousin's garage, leaving him quite sleep-deprived in the evening. This arrangement seems less
            than ideal. His uncle is on the hospital board but that's probably just coincidence. ";
        ],
        npc_avoid freddy_avoid_array,
        npc_walk [; print"slacks";],
        npc_wander_delay 4,
    class Mover MyNPC
    has animate proper transparent;   

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object buzz "Buzz" security_chair
    with name "Buzz",
        take_shrimp false,
        description "He's a tan middle-aged gentleman in his fifties. His hair is close-cropped and he's wearing a stained dark blue 
        security uniform. ",
        daemon [;
            if (self.take_shrimp && TestScope(self)) {
                self.take_shrimp = false;
                print"^Buzz plucks a shrimp from the bowl and plops it into his mouth.^";
                if(shrimp in shrimp_bowl)   {
                    print"^Suddenly, he freezes and beads of sweat break out on his expansive forehead. A loud wet gurgling sound 
                    emanates from his large stomach. Frantically, he bolts up and, with shocking dexterity, grabs
                    his newspaper and dashes out of the room 
                    and disappears.^";
                    remove buzz;
                    remove shrimp;
                    StopDaemon(buzz);
                }
            } else {
                self.take_shrimp = true;
            }
        ],
        signature_desc [;
            "~No, I don't think so. I'm very busy. You should bother somebody else, like a nurse.~";
        ],
        talk_array talk_array_buzz,
        life [;
            give, show:
            if (noun == shrimp) 
            {
                move shrimp to shrimp_bowl;
                "Buzz's eyes widen at the sight of the shrimp.^
                ~Ooh, thought you could get away, huh?~^
                Before you can react, he plops the shrimp in the bowl with the others. ";
            } 
            switch (noun)
            {
                default: "~Sweetie, I'm not interested in ",(the)noun,". I just want to be left alone.~";
            }   
        ],
    class MyNPC
    has animate proper transparent;

Object buzz_newspaper "newspaper" buzz 
    with 
        name 'newspaper' 'paper',
        description "It's the local paper. From here you can only see the 
        hyperbolic headline: ~Blizzard!~.",
        before [;
            read:
            "You can't read it from here and the news bores you anyway. ";
        ],
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object sid_jorry "Sid Jorry" jorry_office
    with name 'sid' 'jorry' 'body' 'corpse',
        description [;
            print"He's a corpulent gentleman with a bushy mustache and a circle of black hair wrapping around
            his otherwise bald head. He always reminded you of your uncle Pete except he's wearing a gray business 
            suit and Uncle Pete only wears black socks and cargo shorts. Jorry is seated at his desk and slumped
            forward across it. Luckily, his eyes are closed because that would be too creepy otherwise. ";
            if (band_aid in self) "You notice a band-aid stuck to the right side of his neck. "; 
                else "You notice a tiny puncure wound on the right side of his neck. ";
        ],
        hide true,
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
    has scenery animate transparent proper;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Array elliot_avoid_array --> room_21 room_22 room_23 stairwell_2 break_room room_31 room_32 room_33;
Object elliot "Elliot" a_ward_1
    with name 'elliot',
        description "He's a muscular tan gentleman in his 30s with spiky black hair. He's rolling idly up and 
        down the hallways in a wheelchair, his right leg bandaged and propped in front of him on a padded strut. ",
        describe [;  
            print"^Elliot is here. ";
            if(self has encountered) "";
            give self encountered;
            ActivateTopic(becky, 308);
            "He's rolling up and down the hallway in his wheelchair. ";
        ],
        signature_desc [;
            "~Sure thing, little lady. I suppose this letter is your ticket out of here.~";
        ],
        life [;
            give, show:
            switch(noun)
            {
                form: add_signature(self); rtrue;
                thriller, air_supply_tape, mixtape: "~Sweet tunes, man.~";
                shrimp: "~Man, you have to throw that away.~";
                default: "He's distracted and doesn't care about ",(the)noun,".";
            }
        ],
        talk_array talk_array_elliot, 
        npc_walk [; print"rolls";],
        npc_wander_delay 4,
        npc_avoid elliot_avoid_array, 
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
Array eugene_avoid_array --> stairwell_b environmental_services engineering central_supply x_ray morgue;
Object eugene "Eugene" hallway_b2
    with name 'eugene' 'gene' 'custodian' 'janitor',
        description "Eugene is the night-shift custodian. He's a short squat balding gentleman wearing gray custodian overalls.
        He's whistling while he pushes a mop in a mop bucket.",
        npc_wander_delay 5,
        npc_avoid eugene_avoid_array,
        npc_walk [; print"pushes the mop bucket";],
        describe [;  
            print"^Eugene is here. ";
            if(self has encountered) "";
            give self encountered;
            "He's the overnight custodian. He waves and stops when he sees you. ~Hey, Candy. I brought a tape for your little tape 
            player. It's on my desk. You kids today don't appreciate real artistry.~";
        ],
        signature_desc [;
            "~Sure, I'll sign. Nobody ever asks the janitor to sign anything.~";
        ],
        life [;
            give, show:
            switch (noun)
            {
                form: add_signature(self); rtrue;
                swipe_card: "~That'll let you down into the sub-basement. But I wouldn't go down there. It's dark and dangerous.~";
                walkman: "~Hey, you can listen to your new tape right here.~";
                thriller: "~'Thriller'? Never heard of it.~";
                air_supply_tape: "~That's for you. It's real music.~";
                shrimp: "~Ohh, that's bad.~";
                default: "Eugene politely feigns interest in ",(the)noun,".";
            }
        ],
        talk_array talk_array_eugene,
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
Object vic "Vic" bathroom
    with name 'vic',
        description "He's tall and thin with slicked-back black hair. His eyes are cold and blue and his cheeks are 
            pock-marked and sunken. ",
        life [;
            give, show:
            if (noun == coaster) { vic_coaster(); rtrue; }
            switch(noun)
            {
                swipe_card: "~I don't think you're supposed to have that, 
                    little lady. Can't have you getting in trouble down here.~ He smiles a sickly leer. ";
                walkman: "~Sure, little lady. You play your music.~";
                default:
                    "Vic smiles a lupine grin. ~Yeah, that's super.~";
            }
        ],
        signature_desc [;
            "~Uh, no. I'm a very new employee, you see. It just wouldn't seem right.~";
        ],
        talk_array talk_array_vic,
        describe [;  
            if (self has encountered)
            {
                rtrue;
            }
            give self encountered;
            SetFlag(F_HAVE_MET_VIC);
            ActivateTopic(nurse_retch, 305);
            "^A tall menacing gentleman is here. You're reminded of a movie that came out earlier this year called ~Scarface~. You only saw the trailer 
            since gangsters aren't your thing. But if there are real hit men out there, this guy certainly would fit the description.  
            He's wearing an ill-fitting hospital maintenance uniform that doesn't appear to be his. Pinned to his shirt there's an ID badge 
            that actually appears to be a ~Chuck E. Cheese~ employee ID with the name ~Vic~ crudely drawn over it in black marker. ";
        ],
        each_turn [;
            if(real_location == sub_basement_02)
            {
                if (IndirectlyContains(player, syringe) || IndirectlyContains(player, denim_jacket) || folding_chair in player) print"^Vic eyes you suspiciously. ~Hey there, 
            little lady. Whatcha got there now?~^";
                if (IndirectlyContains(player, syringe))
                {
                    if (syringe in player) { remove syringe; "He plucks the syringe from you. You suddenly have the sinking feeling your evening is unwinnable. "; }
                    if (syringe in backpack) { remove syringe; "He rudely takes your backpack and rummages through it. He finds the syringe and takes it. He hands 
                    over your backpack but you suddenly have the sinking feeling your evening is unwinnable. "; } 
                    if (syringe in jacket_pocket) { remove syringe; "He takes the jacket and rummages through it. Finding the syringe he smiles. ~You can keep the 
                    jacket,~ he gives it back. I never liked the fellow I took it from.~ You suddenly have the sinking feeling your evening is unwinnable.  "; } 
                }
                if (folding_chair in player) { remove folding_chair; "Can't let you take my chair...";  } 
                print"^Vic is here ";
                print_ret (string) random ("trying to act casual.", "watching you but trying to look like he's not watching you. ",
                "creepily staring at you when he thinks you're not looking. ", "lurking. "); 
            }          
        ],     
    class Mover MyNPC
    has animate proper transparent;

Object vic_badge "badge" vic
    with 
        name 'tag' 'id' 'badge',
        description "It appears to be an employee ID from famed pizza empresario ~Chuck E. Cheese~. Someone has sloppily written ~Vic~ over it with 
            a black marker. ",
        before [;
            take:
            "I don't think he would appreciate you taking his poorly forged employee badge. ";
        ];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object northrup "Dr. Northrup" northrup_office
    with name 'dr' 'northrup' 'doctor' 'walter',  
        description "He's a tall silver-haired gentleman with a patrician bearing. He's wearing a perfectly-creased 
        white lab coat over a crisp shirt and tie. ",
    class Mover MyNPC
    has animate proper;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object mabel "Mabel" main_lobby
    with name 'mabel',
        description "She's an elderly African American lady. She's 82 but her unlined face and white shock of curly hair 
        make her look much younger. She wears a floral print dress with numerous layers against the chill and wire bifocals on a chain. ",
        talk_array talk_array_mabel,
        describe [;  
            print"^Mabel is here, occupying the information desk. ";
            if(self has encountered) "";
            ActivateTopic(buzz, 300);
            give self encountered;
            "She's an elderly lady who volunteers at St. Asterix. Currently, she's manning the information desk. She would have gone 
            home hours ago but the blizzard left her stuck here. She sees you enter and waves with a smile. ";
        ],
        life [;
            give, show:
            switch(noun)
            {
                form: add_signature(self); rtrue;
                kcl_bottle: "~Ooh, girl. That looks like something that shouldn't be.~";
                syringe: "~Oh my, girl. That ain't right. You should report that to somebody.~";
                swipe_card: "She smiles. ~Now how'd you finagle that?~";
                walkman: "She smiles. ~Ooh, my grandkids all have those, too.";
                thriller: "~I actually prefer 'Off The Wall'.~";
                air_supply_tape: "~My husband George sure does like those boys.~";
                shrimp: "~Ooh, girl. You need to throw that rotten thing away before somebody gets sick.~";
                ledger: "She flips through the pages. ~You be careful now, girl. I don't know what this means but I 
                    have a feeling somebody might come after it.~";
                default:
                    "Bemused, she smiles at ",(the)noun,".";
            }
        ],
        signature_desc [;
            "~Sure thing I'll sign, dear. Don't forget, you promised to come back and visit sometime.~";
        ],
    class Mover MyNPC
    has animate female proper;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object becky "Becky" station_a
    with name 'nurse' 'becky',
        description "She's the nurse on duty on Ward A tonight. She's a cheerful blonde in 
        her late 20s and St. Asterix is her first nursing job. She's attired in the standard white nursing 
        uniform. ",
        describe [;  
            print"^Nurse Becky is here ", (string) random ("charting some vital signs. ", "adjusting her white nurse's cap. ",
            "idly playing with a sphygmomanometer. ");
            if(self has encountered) "";
            give self encountered;
            ActivateTopic(mabel, 304);
            ActivateTopic(elliot, 300);
            "She's the nurse on duty tonight on Ward A. ";
        ],
        life [;
            give, show:
            switch(noun)
            {
                form: add_signature(self); rtrue;
                kcl_bottle: "~Huh. Where'd you get that? That shouldn't be outside the pharmacy.~";
                syringe: "~Ooh, that's definitely odd. You should show that to Nurse Retch. She's in charge 
                tonight.~";
                swipe_card: "~That's a security swipe card. I don't think they give those to the candy stripers.~";
                thriller: "~Oh, I LOVE that album.~";
                shrimp: "She holds her nose. ~Oh, get that away.~";
                default:
                    "She politely feigns interest in ",(the)noun,".";
            }
        ],
        signature_desc [;
            "~Oh, ok. Yeah, I guess I can sign your form.~";
        ],
        talk_array talk_array_becky, 
    class Mover MyNPC
    has animate proper female transparent; 

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
Object nurse_retch "Nurse Retch" station_b
    with name 'nurse' 'retch',
        description [;
            if(FlagIsSet(F_BADDIES_FOLLOWING)) "Her eyes seem to almost be glowing like hateful embers and you can clearly see prominent 
                veins lining her temples. ";
                "She's the nurse assigned to Ward B tonight. She's an austere-looking woman in her 40s. Her jet black hair is pulled back into a 
                tight bun and severe green eyes peer out over high gaunt cheekbones. She wears a white 
                nurse uniform with a white nurse cap pinned to her hair. ";
        ],
        describe "^Nurse Retch is here. ",
        talk_array talk_array_retch,
        signature_desc [;
            "~Uh, no. I don't think so. I don't sign off on volunteer forms.~";
        ],
        hide false,
        life [;
            give, show:
            if (noun == syringe) 
            {
                if(FlagIsSet(F_WALKMAN_BLOCKING)) { print"(pressing 'stop' on your walkman.)^"; walkman_playing = false; }
                print"^For an instant you could swear you see a flash of fear cross her face. Then, her thin lips
                relax into a saccharine smile. ~Wherever did you find that, dear? Do be careful, can't 
                have you sticking yourself now, can we?~^";
                self.hide = false;
                nurse_retch.move_mode = TARGET_PATH;
                nurse_retch.target_room = environmental_services;
                StartDaemon(nurse_retch);
                rtrue;
            }
            if (noun == coaster) "Her eyes alight on the coaster. Her lips tighten and there's a brief flash of surprise in her eyes.^^
                ~Why are you carrying around garbage, dear?~";
            if (noun == kcl_bottle)
            {
                if(FlagIsSet(F_WALKMAN_BLOCKING)) { print"(pressing 'stop' on your walkman.)^"; walkman_playing = false; }
                print"She looks at the plastic bottle and a look of pure hate flashes across her face.^^
                ~You are quite the persistent little thing, aren't you? You'd best be careful, candy striper, or you may not 
                finish out your time here.~";
                nurse_retch.move_mode = TARGET_PATH;
	            nurse_retch.target_room = northrup_office;
                StartDaemon(nurse_retch);
                rtrue;
            }
            switch(noun)
            {
                swipe_card: "She eyes you suspiciously. ~Where did you get that? You're not supposed to have that.~"; 
                shrimp: "She grimaces and waves her hand. ";
                form: "She shakes her head. ~No, I think you need to get someone else to sign that.~";
                default:
                print_ret"Annoyed, she ignores ",(the)noun,".";
            }
        ],
        npc_open_door [ _d;
            if (_d == northrup_door)
            {
                self.hide = 1;
                print"Retch unlocks the door and enters the office. ";
            }
        ],
        npc_arrived [;
            if(parent(self)== environmental_services || parent(self) == station_b) 
            { 
                StopDaemon(self); 
                self.move_mode = 0; 
                rtrue; 
            }
            if (parent(self) == northrup_office)
            {
                StopDaemon(self);
                self.move_mode = 0;
                give northrup_door ~open;
                give northrup_door locked;
                move self to northrup_office;
                scope_modified = true;
                print"^She closes the door behind her and you hear a 'click'.^";
                rtrue;
            }
        ],
        each_turn [;
            if (self in environmental_services && player in environmental_services)
            {
                print"^You enter the cluttered room to see Nurse Retch standing next to the supply cabinet and talking to someone on the phone.
                ~Yeah, it's me. We may have a problem. The freaking candy striper of all people just handed me the syringe. 
                He must be getting sloppy. I don't think she knows anything else, though.~ She pauses and there's a 
                voice on the other end. ~Yeah, I know. Just in case, I've stashed the potassium until the storm passes and the 
                cops leave. I'll have a chat with our careless associate later.~^^
                She hangs up the phone and seems to put something in the cabinet. She then turns, surprised, to see you. She nods curtly and 
                leaves the room to the south.^"; 
                move self to hallway_b2;
                move kcl_bottle to storage_cabinet;
                nurse_retch.move_mode = TARGET_PATH;
                nurse_retch.target_room = station_b;
                StartDaemon(nurse_retch);
                nurse_retch.hide = 1;
            }
        ],
    class Mover MyNPC
    has animate female proper transparent;  

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Array winston_avoid_array --> stairwell_2 room room_32 room_33 room_34 room_21 room_22 room_23 room_24;
Object winston "Mr. Winston" hallway_2_3
    with 
        name 'mr' 'winston' 'patient',
        talk_array talk_array_winston,
        description "He's a thin elderly gentleman with sparse strands of white hair on a bald head 
        pocked with age spots. A gray four-day stubble shades his cheeks and chin. He's wearing a 
        hospital gown and slip-proof socks. He pushes a wheeled IV pole in front of him. ",
        describe [;  
            print"^Mr. Winston is here. ";
            if(self has encountered) "";
            give self encountered;
            ActivateTopic(nurse_retch, 303);
            "He's a bored patient pushing an IV pole up and down the wards. ";
        ],
        signature_desc [;
            "~Yeah, I'll sign it. I think you brought me a book once, I think?~";
        ],
        life [;
            give, show:
            switch(noun)
            {
                form: add_signature(self); rtrue;
                walkman: "~Yeah, my wife has one of those for her Wayne Newton tapes.~";
                shrimp: "~I think your dinner has turned.~";
                default:
                "He nods at ",(the)noun," but otherwise seems utterly disinterested. ";
            }
        ],
        npc_walk [; print"pushes his IV pole";],
        npc_wander_delay 6,
        npc_avoid winston_avoid_array, 
    class Mover MyNPC
    has animate proper transparent; 

Object winston_pole "IV pole" winston
    with name 'iv' 'pole',
        description"It's a wheeled metal pole with a hook at the top for hanging IV fluids. There's a clear bag of 
            saline hooked to it. ",
        before [;
            take:
                if (noun == self) "You should leave Winston's IV alone. ";
            ], 
    has static container open transparent;

Object iv_bag "bag of saline" winston_pole 
    with 
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = Nextword();
            if (w1 == 'saline' && w2 == 'bag') return 2;
            if (w1 == 'iv' && w2 == 'bag') return 2;
            if (w1 == 'saline' or 'bag') return 1;
        ],
        description "It's a clear bag of saline. ",
    before [;
        take:
            "You don't need Mr. Winston's bag of salt water. ";
    ],
has static;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object ms_parsons "Ms. Parsons" room_22
    with name 'ms' 'parsons' 'patient',
        description "She's a small pouch of wrinkled skin and scant white hair. She's nearly swallowed up
        by her gown and the bed she's lying in. She's smiling but her eyes are closed. You pause for a moment 
        to make sure she's breathing... Yes. She is. ",
        describe [;
            if (self hasnt encountered) 
            {
                give self encountered;
                ActivateTopic(becky, 306);
                "^Ms. Parsons, a frail hyper-elderly lady is lying in the bed. She's recognized as the oldest resident of the 
                Tri-state area. The best guess is that she's 103 years old. She has dementia and doesn't talk much but is always 
                smiling. She once claimed that the secret to her longevity was that she never married. ";
            }
            "^Ms. Parsons is lying in the bed. ";
        ],
        before [;
            talk:
            "I don't think she can hear you. ";
        ],
        life [;
            give, show:
            "Her eyes are shut and she seems oblivious to ",(the)noun,".";
        ],
    class Mover MyNPC
    has animate female proper transparent;  

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object larry "Larry" room_33
    with name 'larry' 'patient',
        description "He's a jaundiced gentleman in his 60s. He's quite large; the hospital bed barely contains him. 
        He has thick grey hair and his cheeks are ruddy. ",
        describe [;
            if (self hasnt encountered) 
            {
                give self encountered;
                ActivateTopic(nurse_retch, 304);
                "^Larry, a corpulent gentleman is lying in the bed. ";
            }
            "^Larry is lying in the bed. ";
        ],
        signature_desc [;
            "~Yeah, sure. It's too bad they're forcing you to do volunteer work.~";
        ],
        life [;
            give, show:
            switch(noun)
            {
                form: add_signature(self); rtrue;
                walkman, mixtape, thriller, air_supply_tape: "~Yeah, I'm not really much into music.~";
                ledger: "~Hm. That looks like something the police may be interested in.~";
                default:
                "~Sorry, I'm not really interested in that.~";
            }
        ],
        talk_array talk_array_larry, 
    class Mover MyNPC
    has animate proper transparent;  

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object mrs_chen "Mrs. Chen" room_21
    with 
        name 'mrs' 'chen' 'patient',
        description "She's a small birdlike cheery Chinese lady with short gray hair and a constant smile 
            on her face. She doesn't speak English but retains an air of wise insight. ",
        describe [;
            if (self hasnt encountered) 
            {
                give self encountered;
                ActivateTopic(becky, 305);
                "^Mrs. Chen, a small Chinese lady is lying in the bed. ";
            }
            "^Mrs. Chen is lying in the bed. ";
        ],
        signature_desc [;
            "She seems unsure what you want from her but she takes the pen and scrawls something anyway. Smiling sweetly, 
            she hands it back and nods. ";
        ],
        before [;
            talk:
            "She smiles but doesn't seem to speak English. ";
        ],
        life [;
            give, show:
            if (noun == form) { add_signature(self); rtrue; }
            "She just smiles sweetly and nods. ";
        ],
    class Mover MyNPC
    has animate female proper transparent; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object trio "Retch, Northrup, and Vic" 
    with 
        id 0, ! 1 = northrup, 2 = retch, 3 = vic
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'doctor' && w2 == 'northrup') { self.id = 1; return 2; }
            if (w1 == 'nurse' && w2 == 'retch') { self.id = 2; return 2; }
            if (w1 == 'nurse' or 'retch') { self.id = 2; return 1; }
            if (w1 == 'vic') { self.id = 3; return 1; }
            if (w1 == 'northrup' or 'doctor') { self.id = 1; return 1; }
        ],
        npc_open_door [ _d;
            if (_d == scanner_door)
            {
                self.hide = 1;
                print"You see Vic open the door and poke his head through. ";
            }
        ],
        npc_is_following true,
        hide,
        before [;
            talk:
            if (self.id == 1 or 3)
            "He's exercising his right to remain silent. "; 
            if (self.id == 2)
            "She's exercising her right to remain silent. ";
        ],
        life [;
            if (self.id == 1 or 3)
            "He sullenly ignores you. ";
            if(self.id == 2)
            "She sullenly ignores you. ";
        ],
        description [;
            switch (self.id)
            {
                1: "His carefully coifed hair is disheveled and his patrician manner is betrayed by his red face 
                and bulging forehead veins. ";
                2: "She's breathing heavily and has crazy eyes. ";
                3: "He's smiling creepily and has one hand ominously thrust into a pocket. ";
            }
        ],
        npc_post_follow [;
            if (self in real_location)
            {
                trio_catch();
            }
        ],
        npc_post_move [ rm;
            rm = parent(self);
            
            if (ledger in rm) 
            {
                move ledger to trio;
            }   
        ],
    class Mover
    has animate proper pluralname transparent; 

Object trio_follow_timer 
    with 
        time_left,
        time_out [;
            if (FlagIsClear(F_TRIO_IS_FOLLOWING))
            {
                print"^From down the hall you hear Vic holler something about ~getting her~.^";
                trio.move_mode = FOLLOW_PATH;
                SetFlag(F_TRIO_IS_FOLLOWING);
                StartDaemon(trio);  
            }
        ];

Object trio_contact_daemon
    with 
        daemon [;
            if (trio in real_location)
            {
                trio_catch();
                rtrue;
            }
        ];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object injured_trio "Retch, Northrup, and Vic" 
    with 
        id 0, ! 1 = northrup, 2 = retch, 3 = vic
        parse_name [ w1 w2;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'doctor' && w2 == 'northrup') { self.id = 1; return 2; }
            if (w1 == 'nurse' && w2 == 'retch') { self.id = 2; return 2; }
            if (w1 == 'nurse' or 'retch') { self.id = 2; return 1; }
            if (w1 == 'vic') { self.id = 3; return 1; }
            if (w1 == 'northrup' or 'doctor') { self.id = 1; return 1; }
        ],
        describe [; rtrue; ],
        description [;
            switch (self.id)
            {
                1: "His carefully coifed hair is wildly disheveled as he lies, moaning, on the floor. ";
                2: "She's groaning and curled up in a fetal position. ";
                3: "He's lying motionless, eyes closed, groaning in pain. ";
            }
        ],
        before [;
            search:
            switch (self.id) {
                1: 
                    if(parent(letter) == nothing)
                    {
                        move letter to parent(self);
                        print"^You search the incapacitated physician. In an inner pocket you come across a letter. ";
                        <take letter>;
                        rtrue;
                    }
                2:
                    "You pat down the moaning nurse but find nothing of interest. ";
                3:
                    if(parent(switchblade) == nothing)
                    {
                        move switchblade to parent(self);
                        "Cautiously, you search the semi-conscious hit man and find a switchblade knife. ";
                    }
            }
                rtrue;
        ],
    class Mover
    has animate proper pluralname transparent; 





