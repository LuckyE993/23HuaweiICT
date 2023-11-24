################################################################################
# this is used for compile the macos socket
################################################################################

#configure the macos socket itself
EC600S = ${wildcard $(iot_link_root)/network/tcpip/ec600s/*.c}
C_SOURCES += $(EC600S)