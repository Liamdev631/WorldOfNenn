#pragma once
#include "Test.h"
#include "Packets.h"

class Test_Packets : public Test
{
public:
	Test_Packets() 
		: Test("Packet test")
	{

	}
	~Test_Packets() = default;

protected:
	void runAllSubtests() override
	{

		WPacket wpacket(1024);
		size_t offset = 0;

		{
			SP_EntityMoved* data = wpacket.writePacket<SP_EntityMoved>();
			data->move.pos = Vec2s(0x1234, 0x1234);
			data->uid = 0x4321;

			for (size_t i = 0; i < sizeof(SP_EntityMoved); i++)
				if (*(wpacket.getData() + offset + i) != *(u8*)(data + i))
				{
					reportFailure("Error writing SP_EntityMoved packet");
					printf("Memory in wpacket:        "); printBytes(wpacket.getData(), sizeof(SP_EntityMoved), offset);
					printf("Memory in SP_EntityMoved: "); printBytes(data, sizeof(SP_EntityMoved));
					printf("\n"); 
					break;
				}
			offset += sizeof(SP_EntityMoved);
		}

	}
};

