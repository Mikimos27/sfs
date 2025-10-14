#include "../hdr/connection.h"
#include "../hdr/send.h"
#include "../hdr/crypto.h"

#include <cstdio>
#include <cstring>

#include <unistd.h>
#include <sys/time.h>

#include <string>
#include <iostream>


int main(){
    data_packet pack;
    Ed25519 key;
    key.gen_key_pair();
    pack.create_auth(key);
    pack.send_packet(0);
    printf("\nvalid = %s\n", pack.validate_packet(key) == 0 ? "YES" : "NO");
    printf("\n");
    pack.create_msg("HAMMOND, YOU SODDING TICTAC!!!");
    auto worked = *pack.get_data();
    std::cout << "Msg = ";
    for(msglen_t i = 0; i < worked.length(); i++) printf("%c", worked.get_msg()[i]);
    printf("\nMsglen = %d\n", worked.length());
    DH_protocol dh;
    dh.gen_key();
    key.gen_key_pair();
    pack.create_kex(dh, key);
    printf("\n\n");
    pack.send_packet(0);

    return 0;
}
