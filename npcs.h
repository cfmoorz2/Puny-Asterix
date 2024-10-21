Class MyNPC
    with 
        holding [counter i;
            objectloop(i in self && i hasnt worn)   {
                !counter = counter + i.mass;
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
                if(noun == in_obj) { print"You head inside.^"; rfalse; }
                if(noun == out_obj) { print"You head out.^"; rfalse; }
                print"You head off to the ",(name)noun,".^";
            }
        ],
        signature_desc [;
            if(self has female) print"She "; else print"He ";
            print"takes the letter and pen, signs it and hands it back.^";
            return true;
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
                book_cart: "~No thanks. I don't need any romance novels.~";
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
            pants. He's thin with long black hair, long nose, and gaunt cheeks pockmarked with acne. ";
            if (FlagIsSet(F_FREDDY_ASLEEP)) "He's standing here, deeply asleep. "; else 
            "He's making his rounds up and down the hallways. ";
        ],
        life [;
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
                    move dull_key to freddy;
                    scope_modified = true;
                    StopDaemon(freddy);
                    "The tuneful adult contemporary strains of 'Air Supply' wash over the 
                    narcoleptic security guard. His eyes close and he smiles. In another moment he snores and you 
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
            "He's the narcoleptic third-shift security guard. Hiring someone with a sleep-disorder who could nod off at any 
            moment to work the overnight shift seems less than ideal. His uncle is on the hospital board but that's probably just 
            coincidence. ";
        ],
        npc_avoid freddy_avoid_array,
        npc_walk [; print"slacks";],
        npc_wander_delay 3,
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

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object sid_jorry "Sid Jorry" jorry_office
    with name 'sid' 'jorry',
        description [;
            print"He's a corpulent gentleman with a bushy mustache and a circle of black hair wrapping around
            his otherwise bald head. He always reminded you of your uncle Pete except he's wearing a gray business 
            suit and Uncle Pete only wears black socks and cargo shorts. Jorry is seated at his desk and slumped
            forward across it. Luckily, his eyes are closed because that would be too creepy otherwise. ";
            if (band_aid in self) "You notice a band_aid stuck to the right side of his neck. "; 
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
    has scenery animate transparent;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Array elliot_avoid_array --> room_21;! room_22 room_23 stairwell_2 break_room room_31 room_32 room_33;
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
        npc_wander_delay 3,
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
        and is whistling while he pushes a mop in a mop bucket.",
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
                book_cart: "~No thanks. I've got enough books and the missus subscribes to 'People'.~";
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
        description [;
            if (FlagIsSet(F_BADDIES_FOLLOWING)) "He's smiling and seems to be enjoying himself. His right hand is 
            ominously thrust into a pocket."; 
            "He's a tall and thin with slicked-back black hair. His eyes are cold and blue and his cheeks are 
            pock-marked and sunken. He's wearing an ill-fitting hospital maintenance uniform that doesn't appear 
            to be his. ";
        ],
        life [;
            give, show:
            if (noun == coaster) { vic_coaster(); rtrue; }
            switch(noun)
            {
                swipe_card: remove swipe_card; "He plucks the card from you. ~I don't think you're supposed to have that, 
                    little lady. Can't have you getting in trouble down here.~ He smiles a sickly leer. ";
                walkman: "~Sure, little lady. You play your music.~";
                book_cart: "~Yeah, you keep to your books, candy striper. ";
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
                print"^Vic is here. ";
            }
            give self encountered;
            SetFlag(F_HAVE_MET_VIC);
            ActivateTopic(nurse_retch, 305);
            ActivateTopic(worthless, 301);
            "^Vic is here. You're reminded of a movie that came out earlier this year called ~Scarface~. You only saw the trailer 
            since gangsters aren't your thing. But if there are real hit-men out there, this guy certainly would fit the description. ";
        ],
        each_turn [;
            if(real_location == sub_basement_02)
            {
                if (IndirectlyContains(player, syringe) || IndirectlyContains(player, denim_jacket) || folding_chair in player) print"^Vic eyes you suspiciously. ~Hey there, 
            little lady. Whatcha got there now?~^";
                if (IndirectlyContains(player, syringe)) { print "He plucks the syringe from you.^"; remove syringe; }
                if (IndirectlyContains(player, denim_jacket) ) { print "He takes the jacket from you.^"; remove denim_jacket; }
                if (folding_chair in player) { print "He relieves you of the folding chair.^"; remove folding_chair; } 
                print"^Vic is here ";
                print_ret (string) random ("trying to act casual.", "watching you but trying to look like he's not watching you. ",
                "creepily staring at you when he thinks you're not looking. ", "lurking. "); 
            }          
        ],     
    class Mover MyNPC
    has animate proper;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object northrup "Dr. Northrup" northrup_office
    with name 'dr' 'northrup' 'doctor' 'walter',  
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
        describe [;  
            print"^Mabel is here, occupying the information desk. ";
            if(self has encountered) "";
            give self encountered;
            ActivateTopic(buzz, 300);
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
                flashlight: "~That's for you tonight, girl.~";
                walkman: "She smiles. ~Ooh, my grandkids all have those, too.";
                thriller: "~I actually prefer 'Off The Wall'.~";
                air_supply_tape: "~My husband George sure does like those boys.~";
                shrimp: "~Ooh, girl. You need to throw that rotten thing away before somebody gets sick.~";
                ledger: "She flips through the pages. ~You be careful now, girl. I don't know what this means but I 
                    have a feeling somebody might come after it.~";
                book_cart: "~Girl, you know I like perusing the romance books.~";
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
            ActivateTopic(worthless, 300);
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
                book_cart: "She takes a quick peek at the books and magazines. ~No thanks. I think I'm caught up.~";
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
        life [;
            give, show:
            if (noun == syringe) 
            {
                SetFlag(F_RETCH_TRIGGERED);
                StartTimer(retch_timer, 4);
                if(FlagIsSet(F_WALKMAN_BLOCKING)) { print"(pressing 'stop' on your walkman.)^"; walkman_playing = false; }
                print"^For an instant you could swear you see a flash of fear cross her face. Then, her thin lips
                relax into a saccharine smile. ~Wherever did you find that, dear? Do be careful, can't 
                have you sticking yourself now, can we?~ ";
                if (action == ##give)
                {
                    remove syringe;
                    "She accepts the syringe from you and you suddenly have the feeling you maybe shouldn't have done that. ";
                } else {"";}
            }
            if (noun == coaster) { retch_coaster(); rtrue; }
            if (noun == kcl_bottle)
            {
                if(FlagIsSet(F_WALKMAN_BLOCKING)) { print"(pressing 'stop' on your walkman.)^"; walkman_playing = false; }
                print"She looks at the plastic bottle and a look of pure hate flashes across her face.^^
                ~You are quite the persistent little thing, aren't you? You'd best be careful, candystriper, or you may not 
                finish out your time here.~";
                if (action == ##give)
                SetFlag(F_RETCH_KCL);
                nurse_retch.move_mode = TARGET_PATH;
	            nurse_retch.target_room = northrup_office;
                StartDaemon(nurse_retch);
                if (action == ##give)
                {
                    remove kcl_bottle;
                    " She swipes the offered vial from your hand and you suddenly have a sneaking suspicion that you shouldn't 
                    have done that. ";
                } else {"";}
            }
            switch(noun)
            {
                swipe_card: move swipe_card to lost; "She eyes you suspiciously. ~Where did you get that? You're not 
                supposed to have that.~ She plucks the card away. "; 
                shrimp: "She grimaces and waves her hand. ";
                book_cart: "~Yes, you should stick to your duties, candy striper.~";
                default:
                    print_ret"Annoyed, she ignores ",(the)noun,".";
            }
        ],
        npc_arrived [;
            if(parent(self)== environmental_services) 
            { 
                StopDaemon(self); 
                self.move_mode = 0; 
                StartTimer(retch_timer_2, 3);
                rtrue; 
            }
        ],
        each_turn [;
            if (self in environmental_services && player in storage_locker)
            {
                print"^Through the crack in the locker door you see her move to the phone on the wall. She punches an 
                extension and waits a moment.^^
                ~It's me. We may have a problem. The freaking candy striper of all people just handed me the syringe. 
                He must be getting sloppy. I don't think she knows anything else, though.~ She pauses and there's a 
                voice on the other end. ~Yeah, I know. Just in case, I'm stashing the potassium until the storm passes and the 
                cops leave. I'll have a chat with our careless associate later.~^^
                She hangs up the phone and walks to the metal cabinet in the corner. 
                She looks around then pulls on the door handle which won't open. She grunts angrily then balls up a fist, striking 
                a specific spot on the door which pops open.^^
                You see her take something from a pocket, put it in the cabinet, and push the door closed again.^^
                Glancing around one last time, she hurries out of the room to the south.^";
                move self to hallway_b2;
                move kcl_bottle to storage_cabinet;
                SetFlag(F_SAW_KNOCK_SPOT);
                nurse_retch.hide = 1;
                retch_timer_2.time_out();
            }
        ],
    class Mover MyNPC
    has animate female proper transparent;  

Object retch_timer
    with 
        time_left,
        time_out [;
            nurse_retch.move_mode = TARGET_PATH;
	        nurse_retch.target_room = environmental_services;
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
Array winston_avoid_array --> stairwell_2 room room_32 room_33 room_34 room_21 room_22 room_23 room_24;
Object winston "Mr. Winston" hallway_2_3
    with 
        name 'mr' 'winston',
        talk_array talk_array_winston,
        description "He's a thin enderly gentleman with sparse strands of white hair on a bald head 
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
                book_cart: "~Naw, I don't think there's anything left on that cart for me.~";
                default:
                "He nods at ",(the)noun," but otherwise seems utterly disinterested. ";
            }
        ],
        npc_walk [; print"pushes his IV pole";],
        npc_wander_delay 5,
        npc_avoid winston_avoid_array, 
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
Object ms_parsons "Ms. Parsons" room_22
    with name 'ms' 'parsons',
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
    with name 'larry',
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
                book_cart: "~No thanks. I'm just gonna listen to the game.~";
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
        name 'mrs' 'chen',
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
            "She seems unsure what you want from her but she takes the pen and scrawls something, anyway. Smiling sweetly, 
            she hands it back and nods. ";
        ],
        before [;
            talk:
            "She smiles but doesn't seem to speak english. ";
        ],
        life [;
            give, show:
            if (noun == form) { add_signature(self); rtrue; }
            "She just smiles sweetly and nods. ";
        ],
    class Mover MyNPC
    has animate female proper transparent; 

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object worthless "Lt. Worthless" room_23 
    with 
        name 'worthless' 'cop',
        description"He's a paunchy man in his 40s with surprisingly well-coifed black hair. He's wearing 
        a blue patient gown. Somewhow he's attached his badge to the front of it. ",
        hide,
        describe [;
            if (self has encountered)
            {
                "^Lt. Worthless is here, reclining in the bed. ";
            }
            else
            {
                give self encountered;
                "^Lt. Worthless is here, lounging in the bed. He's a local homicide detective who happened to be 
                admitted earlier today for a medical procedure and he's enjoying the attention and intermittent morphine.
                Around the station, he's known for his multiple divorces and not for his acumen as an investigator. ";
            }
        ],
        signature_desc [;
            "~Sure, I'll sign. If you promise to leave me alone and let me enjoy my morphine.~";
        ],
        before [;
            talk:
            if (FlagIsSet(F_ENDGAME)) "He simply hums to himself and smiles. ";
        ],
        life [;
            give, show:
            if (FlagIsSet(F_ENDGAME)) "He's seems more interested in his hands. ";
            if(noun == ledger)
                {
                    print"Unfortunately, Lt. Worthless apparently received some medication recently. He's quietly 
                    humming to himself while he smiles at his hands which are held out in front of him.^^
                    At that moment Lt. Rodriguez enters the room. He's wearing plain-clothes and is 
                    accompanied by two uniformed officers. All three are dusted with snow.^";
                    move rodriguez to room_23;
                    move cop_duo to room_23;
                    rtrue;
                }  
            switch(noun)
            {                  
                syringe: "~Oh, what a surprise,~ he snorts sarcastically. ~Somebody found a syringe in a hospital. Well, 
                    call the FBI. Come see me when you have actual evidence.~";
                kcl_bottle: "~You found a bottle of medicine? So what. I've been taking awesome medicine all day.
                    Don't bother me with acutal evidence.~";
                jorry_tape, walkman:
                "~Yeah, right. Somebody recorded themselves reporting a crime. As if. That only happens on TV.
                    Look, little lady. If you think there's something going on, I need actual evidence.~";
                coaster: "~No thanks. I've got plenty of coasters from the cop bar across the street.~";
                shrimp: "~Aw, man. That's rank. Get that outta here.~";
                form: add_signature(self); rtrue;
                default:
                "The good Lieutenant is completely disinterested in ",(the)noun,".";
            }
        ],
        talk_array talk_array_worthless,
    has animate transparent proper;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object rodriguez "Lt. Rodriguez"
    with 
        name 'rodriguez' 'cop',
        evidence_count 0,
        description "He's a short, trim, dark-skinned gentleman with shirt brown hair
        and piercing dark eyes. ",
        before [;
            talk:
            "He seems a man of few words. ";
        ],
        hide,
        life [;
            give, show:
            if (noun == walkman)
            {
                if (FlagIsSet(F_HEADPHONES_ARE_UNPLUGGED)) "He plugs in the headphones. ";
                if (walkman has open) { print"He closes the tape compartment. "; give walkman ~open; }
                if (~~walkman_playing) { print "He presses the 'play' button. "; walkman_playing = true; }
                print"He puts on the headphones and starts to listen. ";
                if (jorry_tape in walkman)
                {
                    print"His eyes widen as the tape plays, silently to you. After a moment, he stops the walkman, removes the tape and 
                    pockets it.^";
                    move jorry_tape to rodriguez;
                    self.evidence_count++;
                } else 
                "~I don't think this is the tape you want me to hear.~ He hands the walkman back. ";
            }
            switch(noun)
            {
                syringe: move syringe to rodriguez; self.evidence_count++; "He takes the syringe and silently examines it. ";
                kcl_bottle: move kcl_bottle to rodriguez; self.evidence_count++; "He holds up the vial and peers at it closely. ";
                ledger: move ledger to rodriguez; self.evidence_count++; "He quietly flips through the ledger for a few moments. ";
            }
            if (self.evidence_count == 4) you_win();
            rtrue;
        ]
    has animate proper transparent;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object cop_duo "uniformed officers"
    with 
        name 'cops' 'police' 'uniformed' 'officers',
        description "They're a pair of uniformed officers, one tall red-head with a bright bushy mustache, one 
        short, squat, and bald. ",
        hide,
        before [;
            talk:
            "They don't seem very talkative. ";
        ],
        life [;
            "They're pretty much just here for show. ";
        ],
    has pluralname animate transparent;

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
        hide,
        before [;
            talk:
            if (self.id == 1 or 3)
            "He's exercising his right to remain silent. "; "She's exercising her right to remain silent. ";
        ],
        life [;
            if (self.id == 1 or 3)
            "He sullenly ignores you. "; "She sullenly ignores you. ";
        ],
        description [;
            switch (self.id)
            {
                1: "His carefully coifed hair is disheveled and his patrician manner is betrayed by wide eyes 
                and a red face. ";
                2: "Her eyes seem to almost be glowing like hateful embers and you can clearly see prominent 
                veins lining her temples. ";
                3: "He's staring at his feet. ";
            }
        ],

    has animate proper pluralname transparent; 





