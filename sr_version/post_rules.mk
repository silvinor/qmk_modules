#
#  Ideas from https://discord.com/channels/440868230475677696/440868230475677698/1095484933549600808
#  Thank you @tzarc
#

GENERATED_VERSION_H_FILE = $(INTERMEDIATE_OUTPUT)/src/version.h

$(GENERATED_VERSION_H_FILE):
$(info Appending to $(GENERATED_VERSION_H_FILE))
$(shell echo "#define QMK_VERSION_MAJOR $$(git -C "$(TOP_DIR)" describe --tags 2>/dev/null | cut -d. -f1 || echo -1)" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "#define QMK_VERSION_MINOR $$(git -C "$(TOP_DIR)" describe --tags 2>/dev/null | cut -d. -f2 || echo -1)" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "#define QMK_VERSION_PATCH $$(git -C "$(TOP_DIR)" describe --tags 2>/dev/null | cut -d. -f3 | cut -d- -f1 || echo -1)" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "#define QMK_VERSION_DATE \"$$(git -C "$(TOP_DIR)" log -1 --date=short --format=%cd)\"" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "#define QMK_VERSION_DATE_YEAR $$(git -C "$(TOP_DIR)" log -1 --date=format:'%Y' --format=%cd)" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "#define QMK_VERSION_DATE_MONTH $$(git -C "$(TOP_DIR)" log -1 --date=format:'%m' --format=%cd)" >> $(GENERATED_VERSION_H_FILE))
$(shell echo "#define QMK_VERSION_DATE_DAY $$(git -C "$(TOP_DIR)" log -1 --date=format:'%d' --format=%cd)" >> $(GENERATED_VERSION_H_FILE))

generated-files: $(GENERATED_VERSION_H_FILE)
POST_CONFIG_H += $(GENERATED_VERSION_H_FILE)
