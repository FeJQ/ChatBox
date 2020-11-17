import DBHelper

import model.User
import time

db = DBHelper.mySqlHelper()
class CDBManager():
    def __init__(self, *args, **kwargs):
        self.user = model.User.CUser()

    '''
    Summary:获取所有用户        
    Args:
        None
    Return:用户元祖  
    '''        
    def getUserList(self):
        return db.select("select * from CB_User")


    '''
    Summary:登陆验证        
    Args:
        用户对象
    Return:是否成功 
    '''
    def Login(self,_uer):
        user = db.QueryStr("select * from CB_User where User_Name='{0}'".format(_uer.Name))
        if user.Password == _uer.Password:
            return True
        else:
            return False

    '''
    Summary:注册
    Args:
        用户对象
    Return:返回码(0为未知错误,1为成功,2为用户名已被注册)
    '''
    def Register(self,_user):
        count = db.QueryStr("select count(*) from CB_User where User_Name={0}".format(_uer.Name,))
        if count > 0:
            return 2
        else:
            effectRow = db.ExecStr("insert into CB_User(User_Name,User_Password) values(%s,%s)",(_user.Name,_user.Password)) 
            if effectRow > 0:
                return 1
            else:
                return 0


    '''
    Summary:更新用户在线状态
    Args:
        用户名,上线/下线
    Return:受影响行数
    '''
    def UpdateUserState(self,_userName,_ToOnLine):
        return db.ExecStr("update cb_user set User_isOnline={0} where User_Name='{1}'".format(_ToOnLine,_userName))

    '''
    Sunmmary:根据用户ID获取用户名
    Args:
        ID
    Return:用户名
    '''
    def GetUserNameByID(self,UserID):
        data = db.QueryStr("select User_Name from cb_user where User_ID='{0}'".format(UserID))[0]
        return  data

    '''
    Summary:存储消息
    Args:
        接受者和发送者ID,消息内容,是否为群聊
    Return:受影响行数
    '''
    def SaveMessage(self,_SenderID,_ReciverID,_Content,_isForGroup):
        if _isForGroup==1:
            members=self.GetAllGroupMemberID(_ReciverID)
            effectRow=0
            timeStamp=int(round(time.time() * 1000))
            for x in members:
                if(x[0]!=_SenderID):
                    effectRow=effectRow+db.ExecStr("insert into cb_message(Message_SenderID,Message_ReciverID,Message_Content,Message_isForGroup,Message_GroupID,Message_Timestamp)\
                     values({0},{1},'{2}',{3},{4},'{5}')".format(_SenderID,x[0],_Content,_isForGroup,_ReciverID,timeStamp))
            return effectRow;
        else:
            return db.ExecStr("insert into cb_message(Message_SenderID,Message_ReciverID,Message_Content,Message_isForGroup) \
            values({0},{1},'{2}',{3})".format(_SenderID,_ReciverID,_Content,_isForGroup))

    '''
    Summary:获取单聊记录
    Arg:
        接收和发送者ID
    Return:单聊记录
    '''
    def GetFriendChatRecords(self,_SenderID,_ReciverID):
        return db.QueryStr("select * from cb_message where Message_SenderID={0} and Message_ReciverID={1} and Message_isForGroup=0".format(
            _SenderID,_ReciverID
            ))

    '''
    Summary:获取群聊记录
    Arg:
        群组ID
    Return:群聊记录
    '''
    def GetGroupChatRecords(self,_GroupID):
        return db.QueryStr("select * from cb_message where Message_ReciverID={0} and Message_isForGroup=1".format(_GroupID))

    '''
    Sunmmary:获取群组所有成员的ID
    Arg:
        群ID
    Return:组员ID
    '''
    def  GetAllGroupMemberID(self,_GroupID):
        return db.QueryStr("select GroupMember_UserID from cb_groupmember where GroupMember_GroupID={0}".format(_GroupID))