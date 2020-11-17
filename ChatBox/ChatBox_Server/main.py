import chatBox
import threading



# recv =threading.Thread(target=chatBox.ReciveMessage,name="RecvMsg")

# recv.start()


chat=chatBox.Chat()
chat.CreateService()
accept=threading.Thread(target=chat.WaitForClient,name="WaitClient")
accept.start()
chat.ReciveMessage()


