#ARM=arm

ifdef ARM

DEFS = -DPLATFORM_ARM
CC = arm-fsl-linux-gnueabi-gcc

LIBS = -L/usr/local/armlib/alsa/lib/  -lasound 
LIBS += -L/usr/local/armlib/mad/lib/ -lmad
LIBS += -L/usr/local/armlib/paho_mqtt/lib -lpaho-mqtt3a
LIBS += -L/usr/local/armlib/sqlite3/lib -lsqlite3
LIBS += -L/usr/local/armlib/zlog/lib -lzlog
LIBS += -L/usr/local/armlib/openssl/lib -lssl -lcrypto

LOCAL_FLAGS = -D -REENTRANT -lpthread  #flag for pthread lib.

else

CC = gcc

#LIBS = -L/usr/local/x86lib/alsa/lib/ -L/usr/local/x86lib/mad/lib/ -lasound  -lmad
#LIBS += -L/usr/local/x86lib/sqlite3/lib -lsqlite3
LIBS = -L/usr/local/x86lib/zlog/lib -lzlog -lpthread
#LIBS += -L/usr/local/x86lib/paho_mqtt/lib -lpaho-mqtt3c
#LIBS += -L/usr/local/x86lib/openssl/lib -lssl -lcrypto

endif

CFLAGS = -g -Wall -O0 -std=gnu99 -I/usr/local/include/
CFLAGS += -I ./include

INCDIR = include/
SRCDIR = ./
OBJDIR = objs/
LIB_OBJ_DIR = libs_obj/
BINDIR = sbin/
TIME=`date +%Z%Y%m%d_%02k%M%S`
VERSION="\#define PA_HOST_APP_VERSION \"v01.00_"$(TIME)"\" >$(INCDIR)pa_version.h"

#得到运行时依赖的三方库和应用存放路径
libobjs = $(wildcard $(LIB_OBJ_DIR)*)

#变量sources得到src目录下待编译的.c文件的列表
sources = $(wildcard $(SRCDIR)*.c)

sources :=$(filter-out %multicat_receiver-bk.c %ali_mqtt_test.c,$(sources))

#$(warning $(sources))

objects :=$(sources:$(SRCDIR)%.c=$(OBJDIR)%.o)
executes_obj :=$(filter %ali_mqtt_test.o %date_test.o %del_file_by_postfix.o %dir_enum.o %heoo.o \
   %multicat_receiver.o %multicat_receiver_laoliu.o %multicat_receiver-bk.o,$(objects))
executes :=$(executes_obj:$(OBJDIR)%.o=$(BINDIR)%)
common_objects :=$(filter-out $(executes_obj),$(objects))
#$(warning $(objects))
#$(warning $(executes))
TARGET :=$(filter %pa_host,$(objects:$(OBJDIR)%.o=$(BINDIR)%))

#mp3_player :=$(filter %mp3_player,$(objects:$(OBJDIR)%.o=$(BINDIR)%))

mp3_play_api :=$(filter %mp3_play_api.o,$(objects))
#$(warning $(mp3_play_api))

#app_conf = ./pa_host.conf
#zlog_conf = ./pa_log.conf
#db_file = ./PtuAudioDB.db

#得到待生成的.d文件的列表，把objects中每个扩展名.o换成.d即可。也可写成$(patsubst %.o,%.d,$(objects))
dependence := $(objects:.o=.d)

#清除所有文件行尾空格。makefile调用 shell命令中的变量，要用双 $.
#TRIMTAIL = `for file in './*.*';  do sed -i 's/\s*$$//' $$file;  done`

SUBDIRS = ./src/upgrade

#给 target 加密。
ENCRY = `openssl dgst -hmac 'Byd@user1234567890!' -md5 $(TARGET) > $(BINDIR)sspahostappinfo`

#打包 target.
PACK = `zip -r $(BINDIR)paappupdate.zip $(BINDIR)*`

.PHONY : all
all : $(executes)

$(executes): $(common_objects)

$(executes): $(BINDIR)% : $(OBJDIR)%.o
	$(CC) $(CFLAGS) $< $(common_objects) -o $@ $(LOCAL_FLAGS) $(LIBS)


$(objects): $(OBJDIR)%.o : $(SRCDIR)%.c
	$(CC) $(DEFS) $(CFLAGS) -c $< -o $@

#注意该句要放在终极目标all的规则之后，否则.d文件里的规则会被误当作终极规则了
-include $(dependence)

#func gen_dep：由点.c生成 .d文件，产生依赖规则文件。
define gen_dep
	set -e;rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJDIR)\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
endef

#指示对每个.c文件，如何生成其依赖规则文件.d，
$(dependence): $(OBJDIR)%.d : $(SRCDIR)%.c
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	@$(gen_dep)

.PHONY : install
install : $(executes)
	@echo $(ENCRY)
	@echo $(PACK)
	@echo "build end at $(TIME)"

#生成动态版本号。
.PHONY : ver
ver : $(INCDIR)pa_version.h
	@echo "$(VERSION)"

.PHONY : submake
submake :
	@list='$(SUBDIRS)'; for subdir in $$list; do \
        echo "Make in $$subdir"; \
        cd $$subdir && $(MAKE); \
    done

.PHONY : subclean
subclean:
	@list='$(SUBDIRS)'; for subdir in $$list; do \
        echo "Clean in $$subdir"; \
        cd $$subdir && $(MAKE) clean; \
    done

.PHONY : clean
clean:
	rm $(objects) $(executes) $(dependence); \
	rm -rf $(OBJDIR); \
	rm -rf $(BINDIR);
