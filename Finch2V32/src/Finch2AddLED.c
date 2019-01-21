/*
 * Finch2addLED.c
 *
 * Created: 12/11/2018 2:44:41 PM
 *  Author: raghu
 */ 


#include <asf.h>
#include "Finch2AddLED.h" 


COMPILER_ALIGNED(16)
DmacDescriptor example_descriptor SECTION_DMAC_DESCRIPTOR;

struct spi_module spi_master_instance;

void sourceRegisterInit()
{
	uint8_t i =0;
	for(i=0;i<STARTZEROS;i++)
	{
		DMASourceRegister[i]	=	0x00;
	}
	for(i=STARTZEROS;i<STARTZEROS + NOLEDS*NoBytesLED ;i++)
	{
		DMASourceRegister[i]	=	0x88;
	}
	for(i=NOLEDS*NoBytesLED;i<STARTZEROS;i++)
	{
		DMASourceRegister[i]    =   0x00;
	}
	
}

static void transfer_done(struct dma_resource* const resource )
{
	transferDone = true;
}

static void configure_dma_resource(struct dma_resource *resource)
{
	struct dma_resource_config config;
	dma_get_config_defaults(&config);
	config.peripheral_trigger = SERCOM1_DMAC_ID_TX;
	//config.peripheral_trigger = SERCOM0_DMAC_ID_TX;
	config.trigger_action = DMA_TRIGGER_ACTION_BEAT;
	dma_allocate(resource, &config);
}

static void setup_transfer_descriptor(DmacDescriptor *descriptor )
{
	
	uint16_t blockTransferCount = NOLEDS*12 + 2*STARTZEROS;
	struct dma_descriptor_config descriptor_config;
	dma_descriptor_get_config_defaults(&descriptor_config);
	descriptor_config.beat_size = DMA_BEAT_SIZE_BYTE;
	descriptor_config.dst_increment_enable = false;
	descriptor_config.src_increment_enable = true;
	descriptor_config.block_transfer_count = blockTransferCount;
	descriptor_config.source_address = (uint32_t)DMASourceRegister+blockTransferCount;
	descriptor_config.destination_address = 0x42000C28; //sercom1
	//descriptor_config.destination_address = 0x42000828; //sercom0
	descriptor_config.next_descriptor_address = 0;
	dma_descriptor_create(descriptor, &descriptor_config);

}

void setup_DMA()
{
	configure_dma_resource(&example_resource);
	setup_transfer_descriptor(&example_descriptor);
	dma_add_descriptor(&example_resource, &example_descriptor);
	dma_register_callback(&example_resource, transfer_done, DMA_CALLBACK_TRANSFER_DONE);
	dma_enable_callback(&example_resource, DMA_CALLBACK_TRANSFER_DONE);
}

void configureSPIMaster(void)
{
	struct spi_config config_spi_master;
	struct spi_slave_inst_config slave_dev_config;
	spi_get_config_defaults(&config_spi_master);
	config_spi_master.transfer_mode = SPI_TRANSFER_MODE_1;
	config_spi_master.mux_setting = CONF_MASTER_MUX_SETTING;
	config_spi_master.pinmux_pad0 = CONF_MASTER_PINMUX_PAD0;
	config_spi_master.pinmux_pad1 = CONF_MASTER_PINMUX_PAD1;
	config_spi_master.pinmux_pad2 = CONF_MASTER_PINMUX_PAD2;
	config_spi_master.pinmux_pad3 = CONF_MASTER_PINMUX_PAD3;
	config_spi_master.receiver_enable  = false;
	config_spi_master.mode_specific.master.baudrate =  SPIBAUDRATE; //2.5MHz  = 400ns per signal
	config_spi_master.generator_source = GCLK_GENERATOR_3;
	spi_init(&spi_master_instance, CONF_MASTER_SPI_MODULE, &config_spi_master);
	spi_enable(&spi_master_instance);
}

void SPIMasterInit()
{
	configureSPIMaster();
}

void RGBPI55Init()
{
	sourceRegisterInit();
	SPIMasterInit();
	setup_DMA();
}