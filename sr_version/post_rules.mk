#
#  Ideas from https://discord.com/channels/440868230475677696/440868230475677698/1095484933549600808
#  Thank you @tzarc & @elpekenin
#  * This should work for both QMK and Vial-QMK
#

GENERATED_VERSION_H_FILE = $(INTERMEDIATE_OUTPUT)/src/version.h

$(GENERATED_VERSION_H_FILE):

# Info

DUMMY := $(shell printf "\033[1;34mGently poking at the $(GENERATED_VERSION_H_FILE) file...\033[0m\n")
$(info $(DUMMY))

# Date

QMK_GIT_DATE := $(shell git -C "$(TOP_DIR)" log -1 --date=format:'%Y-%m-%d' --format=%cd 2>/dev/null)
ifeq ($(QMK_GIT_DATE),)
# not on git, or git tag broken
QMK_VERSION_DATE := "unknown"
QMK_VERSION_DATE_YEAR := -1
QMK_VERSION_DATE_MONTH := -1
QMK_VERSION_DATE_DAY := -1
else
QMK_VERSION_DATE := "$(QMK_GIT_DATE)"
QMK_VERSION_DATE_YEAR := $(shell echo $(QMK_GIT_DATE) | cut -d- -f1)
QMK_VERSION_DATE_MONTH := $(shell echo $(QMK_GIT_DATE) | cut -d- -f2)
QMK_VERSION_DATE_DAY := $(shell echo $(QMK_GIT_DATE) | cut -d- -f3)
endif

# Version

QMK_GIT_VERSION := $(shell git -C "$(TOP_DIR)" describe --tags 2>/dev/null)
ifeq ($(QMK_GIT_VERSION),)
# account for Vial missing git tag
QMK_VERSION_MAJOR := $(shell echo $(QMK_VERSION_DATE_YEAR) | sed 's/.*\(..\)$$/\1/')
QMK_VERSION_MINOR := $(QMK_VERSION_DATE_MONTH)
QMK_VERSION_PATCH := $(QMK_VERSION_DATE_DAY)
else
QMK_VERSION_MAJOR := $(shell echo $(QMK_GIT_VERSION) | cut -d. -f1)
QMK_VERSION_MINOR := $(shell echo $(QMK_GIT_VERSION) | cut -d. -f2)
QMK_VERSION_PATCH := $(shell echo $(QMK_GIT_VERSION) | cut -d. -f3 | cut -d- -f1)
endif
QMK_VERSION_BCD := $(shell printf "0x%02X%02X%04X" \
  $(QMK_VERSION_MAJOR) \
  $(QMK_VERSION_MINOR) \
  $(QMK_VERSION_PATCH))

# Output

$(shell echo "" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "/* --- Liberally meddled with by the SilvinoR/SR_Version Community Module --- */" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "#define QMK_VERSION_BCD $(QMK_VERSION_BCD)" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "#define QMK_VERSION_MAJOR $(QMK_VERSION_MAJOR)" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "#define QMK_VERSION_MINOR $(QMK_VERSION_MINOR)" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "#define QMK_VERSION_PATCH $(QMK_VERSION_PATCH)" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "#define QMK_VERSION_DATE \"$(QMK_VERSION_DATE)\"" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "#define QMK_VERSION_DATE_YEAR $(QMK_VERSION_DATE_YEAR)" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "#define QMK_VERSION_DATE_MONTH $(QMK_VERSION_DATE_MONTH)" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "#define QMK_VERSION_DATE_DAY $(QMK_VERSION_DATE_DAY)" >> $(GENERATED_VERSION_H_FILE))

# /done

generated-files: $(GENERATED_VERSION_H_FILE)
POST_CONFIG_H += $(GENERATED_VERSION_H_FILE)
