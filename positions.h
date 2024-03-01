class Entry
{
public:
    int offset, length;
    Entry(int _offset, int _length){
        this->offset = _offset;
        this->length = _length;
    }
};

class Binary_Packet_Header 
{
public:
    Entry msg_seq_number = Entry(42, 4);
    Entry sending_time = Entry(46, 8);
};

class Message_Header{
    public:
    Entry block_len;
    Entry template_id;
    Entry Schema_id;
    Entry Version;
};

class Message_Content{

};

class SnapShotGroups {
    class GroupSize{
        Entry blockLen;
        Entry numInGroup;
    };
};

class SnapShotMessage: public Message_Content{
    Entry security_id;
};

class Message{
    public:
    Entry message_size;
    Message_Header message_header;
    Message_Content SnapShotMessage;

};


class Snapshot_Refresh_Book
{
public:
    Binary_Packet_Header binary_packet_header;
};

