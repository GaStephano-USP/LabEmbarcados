from sqlalchemy import create_engine, Column, Integer, Sequence, Float, ARRAY, DateTime
from sqlalchemy.sql import func  # Importando a função 'func' para utilizar 'now'
from sqlalchemy.ext.declarative import declarative_base

# 1. Definir a URL de conexão
# Substitua pelos detalhes do seu PostgreSQL
DATABASE_URL = "postgresql+psycopg2://postgres:1234@localhost:5432/postgres"

# 2. Criar uma engine para o PostgreSQL
engine = create_engine(DATABASE_URL, echo=True)

# 3. Criar a base para o ORM
Base = declarative_base()
    
# 4. Definir uma tabela usando SQLAlchemy ORM
class Entries(Base):
    __tablename__ = 'entries'
    
    id = Column(Integer, Sequence('usuario_id_seq'), primary_key=True)
    temperature = Column(Float)
    oximetry = Column(Integer)
    bpm = Column(Integer)
    ecg = Column(ARRAY(Integer))
    timestamp = Column(DateTime, default=func.now()) ##sempre coloca o datetime atual ao inserir um dado com func.now

# 5. Criar o banco de dados (tabelas)
# Esta linha cria todas as tabelas que herdam de Base, ou seja, 'usuarios' neste exemplo
# Cria se a tabela não existe no banco de dados
Base.metadata.create_all(engine)