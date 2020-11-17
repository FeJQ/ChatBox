import socket
import time,datetime
import client
import message
import struct
import DBManager
import threading



# 客户端
Clients = dict()

# 服务端
server = None

# 🔒客户端列表锁
lock = threading.Lock()



class Chat():
    def __init__(self):
        #实例化 CDBManager()
         self.sql = DBManager.CDBManager()

    # Summary:
    #       创建服务端
    # Args:
    #       None
    # Return:
    #       None
    def CreateService(self):
        # 创建服务器套接字
        global server
        server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        server.setblocking(False)
        # 绑定IP和端口
        server.bind(("127.0.0.1",60556))
    
        # 开始监听
        server.listen(5)
        # server.setblocking(False)
    
    
    # Summary:
    #       等待客户端连接
    # Args:
    #       None
    # Return:
    #       None
    def WaitForClient(self):
        while True:      
            try:
                # 接收客户端请求
                client_socket, addr = server.accept()
                print("addr:{0} 已连接".format(addr))
                # 添加Client对象到Clients
                Clients["SYSTEM"] = client_socket
                time.sleep(0.3)
            except :
                pass       
    
         

    # Summary:
    #     发送消息
    # Args:
    #     None
    # Return:
    #    None
    def SendMessage(self,msg,reciverName):
        try:
            name=reciverName[0]
            if  name in Clients.keys():
                if Clients[name]!='':
                    Clients[name].send(msg)
                    print("发送成功!")                    
        except BaseException as e:
            print(e)
        # client.send(msg.encode("utf8"))
    

    # Summary:
    #     接收消息
    # Args:
    #     None
    # Return:
    #     None
    def ReciveMessage(self):
        while True:
            lock.acquire()
            Name=''
            for key,val in Clients.items():  
                try:
                    # 接收消息包
                    if val!='':
                        msg = val.recv(2116)                
                        Name=self.MessagePip(msg)                    
                # 正常错误捕获(非阻塞的套接字无法立即获取消息)
                except BlockingIOError as e:
                    pass
                # 连接断开错误捕获
                except ConnectionResetError as e:
                    # print("连接断开")
                    # print(Clients)
                    val=''
                    self.sql.UpdateUserState(key,0)
                # 其他错误
                except BaseException as e:
                    print("错误:{0}",e)
            if(Name!=''):
                Clients[Name]=Clients["SYSTEM"]
                Name=''
                Clients["SYSTEM"]=""
            lock.release()          
            time.sleep(0.3)
    
    
    # Summary:
    #     消息管道
    # Args:
    #     msg:客户端发送过来的消息包
    # Return:
    #     None
    def MessagePip(self,msg):       
        # 将消息进行解码
        PackFomat="i" + "l" + "16s" + "i" + "16s" + "i" + "2048s" + "i" + "l" + "l"+"i"+"l"
        pack,\
        sendTime,\
        userName,\
        userNameLength,\
        password,\
        passwordLength,\
        msgContent,\
        msgLength,\
        senderID,\
        reciverID, \
        isGroup,\
        GroupID= struct.unpack(PackFomat,msg)
        userName = userName[0:userNameLength].decode("utf8")
        password = password[0:passwordLength].decode("utf8")
        msgContent = msgContent[0:msgLength * 2].decode("utf16")
    
        ### 识别消息类型
        # 注册
        if pack == message.MessagePack.PACK_Register.value:
            pass
        # 登陆
        if pack == message.MessagePack.PACK_Login.value:
            # item[userName]=
            # Clients[userName]=Clients["SYSTEM"]
            print("{1}用户{0}已登录".format(userName,datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')))
            self.sql.UpdateUserState(userName,1)
            return userName
        # 接发消息
        if pack == message.MessagePack.PACK_Message.value:            
            if isGroup==0:
                reciverName=self.sql.GetUserNameByID(reciverID)
                self.SendMessage(msg,reciverName)
                self.sql.SaveMessage(senderID,reciverID,msgContent,isGroup)
            else:
                members=self.sql.GetAllGroupMemberID(GroupID)
                for x in members:
                    if x[0]==senderID:
                        continue
                    reciverName=self.sql.GetUserNameByID(x[0])
                    self.SendMessage(msg,reciverName)
                self.sql.SaveMessage(senderID,GroupID,msgContent,isGroup)
        # 验证消息
        if pack==3:
            if isGroup!=0:
                pass
            else:
                n=self.sql.GetUserNameByID(reciverID)
                self.SendMessage(msg,n)
        return ""
