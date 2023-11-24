################################################################################
# this is used for compile the macos socket
################################################################################

#configure the macos socket itself
M26 = ${wildcard $(iot_link_root)/network/tcpip/m26/*.c}
C_SOURCES += $(M26)