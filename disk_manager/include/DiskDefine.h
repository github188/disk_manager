#ifndef DISK_DEFINE_H
#define DISK_DEFINE_H

#if __BYTE_ORDER == __BIG_ENDIAN //(1)

#ifdef __le16_to_cpu //(2)

#define CF_LE_W	__le16_to_cpu
#define CF_LE_L	__le32_to_cpu
#define CT_LE_W	__cpu_to_le16
#define CT_LE_L	__cpu_to_le32
#else //(2)
#define CF_LE_W(v) ((((v) & 0xff) << 8) | (((v) >> 8) & 0xff))
#define CF_LE_L(v) (((unsigned)(v)>>24) | (((unsigned)(v)>>8)&0xff00) | \
               (((unsigned)(v)<<8)&0xff0000) | ((unsigned)(v)<<24))
#define CT_LE_W(v) CF_LE_W(v)
#define CT_LE_L(v) CF_LE_L(v)
#endif /* defined(__le16_to_cpu) */  //(2)
    
#else //(1)

#define CF_LE_W(v) (v)
#define CF_LE_L(v) (v)
#define CT_LE_W(v) (v)
#define CT_LE_L(v) (v)

#endif /* __BIG_ENDIAN */  //(1)

#define HDIO_GETGEO		        0x0301	/* get device geometry */
#define BOOTCODE_FAT32_SIZE	    420
#define BOOTCODE_SIZE		    448
#define DEFAULT_SECTOR_SIZE     512
#define DBR_RESERVED_SECTORS    32
#define DEFAULT_FAT_NUM         2
#define MSDOS_FAT32_SIGN        "FAT32   " 
#define BOOT_SIGN               0xAA55 
#define SECTORS_PER_BLOCK       ( BLOCK_SIZE / DEFAULT_SECTOR_SIZE )
#define MAX_CLUST_32            ((1 << 28) - 16) 
#define ATTR_VOLUME             8 

#define LONGLONG 	unsigned long long 
#define LONG 		unsigned long
#define __u8   		unsigned char
#define __u16  		unsigned short int
#define __u32  		unsigned int
#define __u64  		unsigned long long
#define BYTE   		unsigned char

typedef struct hd_geometry 
{
      unsigned char heads;
      unsigned char sectors;
      unsigned short cylinders;
      unsigned long start;
}hd_geometry;


typedef struct msdos_dir_entry //Ŀ¼��ṹ,32���ֽ�
{
    char	name[8], ext[3];	/* name and extension */
    __u8        attr;			/* attribute bits */
    __u8	lcase;			/* Case for base and extension */
    __u8	ctime_ms;		/* Creation time, milliseconds */
    __u16	ctime;			/* Creation time */
    __u16	cdate;			/* Creation date */
    __u16	adate;			/* Last access date */
    __u16	starthi;		/* high 16 bits of first cl. (FAT32) */
    __u16	time, date, start;	/* time, date and first cluster */
    __u32	size;			/* file size (in bytes) */
}msdos_dir_entry;

typedef struct long_msdos_dir_entry //��Ŀ¼��ṹ,32*3���ֽ�
{
	char long_msdos_dir[2][32];	   
	msdos_dir_entry dir_entry;    
}long_msdos_dir_entry;

typedef struct fat32_fsinfo //32���ֽ�
{
  __u32		reserved1;	/* Nothing as far as I can tell */
  __u32		signature;	/* 0x61417272L */
  __u32		free_clusters;	/* Free cluster count.  -1 if unknown */
  __u32		next_cluster;	/* Most recently allocated cluster.
				 * Unused under Linux. */
  __u32		reserved2[4];
}fat32_fsinfo;


typedef struct msdos_volume_info   //26���ֽ�
{
	__u8		drive_number;	/* BIOS drive number */
	__u8		RESERVED;	/* Unused */
	__u8		ext_boot_sign;	/* 0x29 if fields below exist (DOS 3.3+) */
	__u8		volume_id[4];	/* Volume ID number */
	__u8		volume_label[11];/* Volume label */
	__u8		fs_type[8];	/* Typically FAT12 or FAT16 */
}msdos_volume_info;

typedef struct _fat32
{
  __u32	fat32_length;	/* sectors/FAT */
  __u16	flags;		/* bit 8: fat mirroring, low 4: active fat */
  __u8	version[2];	/* major, minor filesystem version */
  __u32	root_cluster;	/* first cluster in root directory */
  __u16	info_sector;	/* filesystem info sector */
  __u16	backup_boot;	/* backup boot sector */
  __u16	reserved2[6];	/* Unused */
  msdos_volume_info vi;
  __u8	boot_code[BOOTCODE_FAT32_SIZE];//420���ֽ�+54=474
}_fat32;

typedef struct _oldfat
{
	msdos_volume_info vi;//26���ֽ�
	__u8	boot_code[BOOTCODE_SIZE];//448���ֽ�+26=474
}_oldfat;

typedef union _fstype
{

  _fat32 fat32;
 _oldfat  oldfat;
  
}_fstype;

typedef struct msdos_boot_sector//512���ֽ�
{
	__u8	        boot_jump[3];	/* Boot strap short or near jump */
	__u8          system_id[8];	/* Name - can be used to special case
				   partition manager volumes */
	__u8          sector_size[2];	/* bytes per logical sector */
	__u8          cluster_size;	/* sectors/cluster */
	__u16         reserved;	/* reserved sectors */
	__u8          fats;		/* number of FATs */
	__u8          dir_entries[2];	/* root directory entries */
	__u8          sectors[2];	/* number of sectors */
	__u8          media;		/* media code (unused) */
	__u16         fat_length;	/* sectors/FAT */
	__u16         secs_track;	/* sectors per track */
	__u16         heads;		/* number of heads */
	__u32         hidden;		/* hidden sectors (unused) */
	__u32         total_sect;	/* number of sectors (if sectors == 0) */
	_fat32 		  fat32;
	//_oldfat  	  oldfat;
	__u16		boot_sign;
} msdos_boot_sector;

//////////////////////////////////////////////////////////////////////////////////

#define LONG_MSDOS_DIR_SUPPORT 1   //֧�ֳ��ļ���

#if LONG_MSDOS_DIR_SUPPORT
//��ǰ���֧��26���ַ��ĳ��ļ���(fat32���֧��255���ַ���������)
#define LONG_DIR_ITEM_SIZE     	(32*3)  
#else
#define LONG_DIR_ITEM_SIZE     	 32
#endif

#ifdef __cplusplus
extern "C"{
#endif

#define SDMOUNTPOINT      		"/opt/httpServer/lighttpd/htdocs/sd/ipc"
#define SDDirName         		"/opt/httpServer/lighttpd/htdocs/sd" 

#define FILE_MAX_LENTH     		(2*1024*1024)   	//�ƴ洢¼6�������ݣ���������Ƶ
#define MP4_MAX_LENTH     		(8*1024*1024)   	//MP4ÿ���ļ����ռ8M
#define JPEG_MAX_LENTH     		(384*1024)        	//jpegÿ���ļ����ռ384k

#define FLAG_INDEX_HEAD    		0XAAF0F0AB     		//����ͷ��־,�����������־����û����������ʼԤ����
#define MAX_INTERVAL_TIME 		1					//�����ļ�ʱ�������1s�ڣ�����Ϊ�������ļ�����
#define DateListMax             100
//����������(index)
//--------------------------------------------
//HeadIndex | gMp4IndexList | gJpegIndexList | 
//--------------------------------------------

enum RECORD_FILE_TYPE 
{
    RECORD_FILE_TS,
    RECORD_FILE_MP4,
    RECORD_FILE_FLV,
    RECORD_FILE_JPG,
    RECORD_FILE_AVI,
    RECORD_FILE_H264,
    RECORD_FILE_MAX
};

char* pFileTypeString[RECORD_FILE_MAX]=
{
    "z.ts",
    ".mp4",
    ".flv",
    ".jpg",
    ".avi",
    ".H264"
};

//�ļ�״̬
enum RECORD_FILE_STATE  
{
	WRITE_OK = 0,		   	//���ļ�
	NON_EMPTY_OK,	   		//�ǿ��ļ�
	OCCUPATION,    			//�ļ���ռ��
	REST,			   		//��
};

enum RECORD_TYPE
{
    UNKNOWN_RECORD = 0,
    EVENT_RECORD,			//�¼�¼��
    MANUAL_RECORD,			//�ֶ�¼��
    SCHEDULE_RECORD,		//�ƻ�¼��
};

enum ALARM_TYPE
{
    UNKNOWN = 0,
    VIDEO_MOTION,
    PIR_MOTION,
    PIR_VIDEO_MOTION, 
    AUDIO_MOTION, 
    IO_ALARM,
    LOW_TEMP_ALARM,
    HIGH_TEMP_ALARM,
    LOW_HUM_ALARM,
    HIGH_HUM_ALARM,
    LOW_WBGT_ALARM,
    HIGH_WBGT_ALARM,
    CALLING   
};

//�ļ���Ϣ
typedef struct FileInfo
{	
	__u16 fileIndex;						//�ļ����(������)
	__u16 FileFpart;						//�ļ�Ԥ����־
	//__u16 recordDuration;    				//¼��ʱ��(��λ:��)
	enum RECORD_FILE_STATE filestate;		//�ļ�״̬	
	//enum RECORD_TYPE recordType;			//¼������(�ƻ�¼���ֶ�¼���¼�¼��)
	enum ALARM_TYPE  alarmType;             //��������	
	//enum RECORD_FILE_TYPE fileType;         //�ļ�����
	__u16 recordStartDate;                  //¼��ʼ����
	__u16 recordStartTime;                  //¼��ʼʱ��
	__u32 recordStartTimeStamp;				//¼��ʼʱ���
	__u32 recordEndTimeStamp;				//¼�����ʱ���
	LONG  fileSize;                 		//�ļ�����
}FileInfo;
/*	
	����=(���-1980)*512+�·�*32+��  (2���ֽ�)
	0x18�ֽ�0~4λ����������0x18�ֽ�5~7λ��0x19�ֽ�0λ���·ݣ�0x19�ֽڵ�1~7λΪ���

	ʱ��=Сʱ*2048+����*32+��/2   
	0x16�ֽڵ�0~4λ����2��Ϊ��λ����ֵ��
	0x16�ֽڵ�5~7λ��0x17�ֽڵ�0~2λ�Ƿ��ӣ�0x17�ֽڵ�3~7λ��Сʱ   (2���ֽ�)
*/
typedef struct DateList   //�����б�
{
	__u16 mDate;      	//����
	__u16 mCounts;		//����
}DateList;

typedef struct GosIndex   //�������� 
{
	__u32 startCluster;				//��ʼ��
	__u32 CluSectorsNum;  			//�����ڵ�����
	__u32 CluSectorsEA;  			//�����ڵ�������ƫ��
	__u64 DirSectorsNum;   			//Ŀ¼���ڵ�����
	__u32 DirSectorsEA;				//Ŀ¼���ڵ�����ƫ��
	__u64 DataSectorsNum;			//�ļ����ݶ�Ӧ���ڵ���ʼ����
	__u64 DataSectorsEA;          	//�ļ��ڴ����е�ƫ��(�ļ���λ�ò���)
	FileInfo fileInfo;
}GosIndex;

typedef struct HeadIndex   //����ͷ
{
	LONG  FlagIndexHead;           	//����ͷ��־
	__u32 FAT1StartSector;		   	//fat1����ʼ����
	__u32 RootStartSector;			//��Ŀ¼��ʼ����
	__u64 ChildStartSector;      	//��Ŀ¼��ʼ����  
	__u32 ChildStartCluster;        //��Ŀ¼��ʼ��
	__u64 ChildItemEA;              //��Ŀ¼����Ŀ���ڵĵ�ַƫ��
	__u64 ChildClusterListEA;       //��Ŀ¼�Ĵ�����ַƫ��
	__u32 lRootDirFileNum;			//mp4�ļ�����(���������ļ�)
	__u32 lJpegFileNum;             //jpeg�ļ�����
	__u32 HeadStartSector;   		//����ͷ�ڴ��̵���ʼ����
	__u32 JpegStartEA;           	//jpeg�����ĵ�ַƫ��
	__u32 CurrIndexPos;				//���浱ǰʹ�õ��������������´�ֱ��˳�����
	__u8  ClusterSize;              //�صĵ�λ(����/��)
}HeadIndex;


#ifdef __cplusplus
}
#endif

#endif
