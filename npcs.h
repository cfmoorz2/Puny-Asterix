Class MyNPC
    with 
        max_capacity 24,
        holding [counter i;
            objectloop(i in self && i hasnt worn)   {
                !counter = counter + i.mass;
            }
            return counter;
        ],
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
            "He's the morgue assistant in charge of your body before it can be picked up in the morning. ";
        ],
    class Mover MyNPC
    has animate proper transparent;     