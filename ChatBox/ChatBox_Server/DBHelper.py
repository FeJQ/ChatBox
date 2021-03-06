import MySQLdb
import time
import config


class mySqlHelper(object):
    def __init__(self):
        #获取数据库连接信息
        self.conn = config.conn 

    # 执行sql语句,并返回受影响行数
    def ExecStr(self,sql,*args):
        conn=MySQLdb.connect(**self.conn)
        cur=conn.cursor()
        effectRow=0
        try:                    
            if args==None:
                cur.execute(sql)
                conn.commit()
            else:
                cur.execute(sql,args)
                conn.commit()
            effectRow=cur.rowcount            
        except BaseException as e:
            print("{0}  {1}".format(time.strftime('%Y/%m/%d %H:%m:%s',time.localtime(time.time())),e))
            conn.rollback()
        finally:
            cur.close()
            conn.close()
            return effectRow

    # 执行sql语句,并返回查询结果
    def QueryStr(self,sql,*args):
        conn = MySQLdb.connect(**self.conn)
        cur = conn.cursor()
        if args==None:
            cur.execute(sql)
        else:
            cur.execute(sql,args)
        data = cur.fetchall()
        cur.close()
        conn.close()
        return data



    ###### 以下内容为网摘(不明白为什么要写这么多) #######

    #创建表
    def createTable(self,sql):
        conn = MySQLdb.connect(**self.conn)
        cur=conn.cursor()
        cur.execute(sql)
        cur.close()
        conn.close()

    #查询单行 sql:为SQL语句  prames:为要传入的参数，可以为元组
    def selectOne(self,sql,prames):
        #传入的数据库链接信息是以字典的形式故而 **self.conn
        conn = MySQLdb.connect(**self.conn)
        cur = conn.cursor() 
        #将查询数据结果以字典形式呈现
        #cur = conn.cursor(cursorclass = MySQLdb)
        cur.execute(sql,prames)
        #匹配单条数据
        data = cur.fetchone()
        cur.close()
        conn.close()
        return data #返回执行sql语句得到的数据，查询一般用得到。

    #查询全部数据
    def selectall(self,sql):
        conn = MySQLdb.connect(**self.conn)
        cur = conn.cursor()
        #cur = conn.cursor(cursorclass = MySQLdb)
        cur.execute(sql)
        #data = cur.fetchmany(number)接受number条数据返回
        #featchall匹配多条数据
        data = cur.fetchall()
        cur.close()
        conn.close()
        return data

    #查询多条数据
    def selectmany(self,sql,prames,number):
        conn = MySQLdb.connect(**self.conn)
        cur = conn.cursor()
        cur.execute(sql,prames)
        #接受number条数据返回
        data = cur.fetchmany(number)
        cur.close()
        conn.close()
        return data

    #插入数据 
    def insert(self,sql,prames):
        conn = MySQLdb.connect(**self.conn)
        cur = conn.cursor()
        recount = cur.execute(sql,prames)
        conn.commit()
        cur.close()
        conn.close()
        return recount

    #插入多条数据
    def insertMany(self,sql,lis):
        conn = MySQLdb.connect(**self.conn)
        #conn.ping(True)
        cur = conn.cursor()
        #executemany一次处理多条数据
        recount = cur.executemany(sql,lis)
        conn.commit()
        cur.close()
        conn.close()
        return recount

    #删除数据
    def delete(self,sql,prames):
        conn = MySQLdb.connect(**self.conn)
        cur = conn.cursor()
        recount = cur.execute(sql,prames)
        conn.commit()
        cur.close()
        conn.close()
        return recount

    #删除全部数据
    def deleteAll(self,sql):
        conn = MySQLdb.connect(**self.conn)
        cur = conn.cursor()
        recount = cur.execute(sql)
        conn.commit()
        cur.close()
        conn.close()
        return recount

    #更新数据
    def update(self,sql,prames):
        conn = MySQLdb.connect(**self.conn)
        cur = conn.cursor()
        recount = cur.execute(sql,prames)
        conn.commit()
        cur.close()
        conn.close()
        return recount        
