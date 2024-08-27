 INT32 SockFd；

 //第一个参数，协议簇，填写PF_PACKET
 //
 ////第二个参数，填写SOCK_RAW,表明这是原始socket，这样数据包就不会经过协议栈的处理了。
 //
 ////第三个参数，希望接受到的消息类型。参考If_ether.h中的协议定义。这里实际上也可以自己定义。
 //
 ////它表明了上层协议的类型（注意，我们现在是在直接和网卡打交道）。
 //
 ////内核会去判断消息头里面的类型，如果匹配，就往应用层发，如果不匹配，就不发。
 //
 ////ETH_P_ALL表示不管什么类型的协议都往应用层发。
 //
 //SockFd = socket(PF_PACKET, SOCK_RAW, htons(VSTRONG_PROTOCOL));
 //
 //if (-1 == SockFd)
 //{
 //  DEBUGMSG(1,("Level:%d [Searcher.main] create socket error.\n", ERR));
 //    return 1;
 //    }
 //
 //    char szBuff[2048] = {0};
 //
 //    //不绑定网卡，不绑定地址，来了的数据包都接收
 //
 //    i32Len = recvfrom(SockFd, szBuff, sizeof(szBuff), 0, NULL, NULL);
 //    if ( i32Len < 14 )
 //    {
 //     //接收的数据还不到一个帧头
 //      DEBUGMSG(1,("Level:%d [Searcher.main]recvfrom returns %d \n", ERR,i32Len));
 //       return 1；
 //       }
