from sqlalchemy import create_engine, Column, Integer, Sequence, Float, ARRAY
from sqlalchemy.orm import DeclarativeBase, MappedAsDataclass

# 1. Definir a URL de conexão
# Substitua pelos detalhes do seu PostgreSQL
DATABASE_URL = "postgresql+psycopg2://postgres:1234@localhost:5432/postgres"

# 2. Criar uma engine para o PostgreSQL
engine = create_engine(DATABASE_URL, echo=True)

# 3. Criar a base para o ORM
class Base:
    __allow_unmapped__ = True
    
class Base(DeclarativeBase, MappedAsDataclass):
    pass

# 4. Definir uma tabela usando SQLAlchemy ORM
class Entries(Base):
    __tablename__ = 'entries'
    
    id = Column(Integer, Sequence('usuario_id_seq'), primary_key=True)
    temperature = Column(Float)
    oximetry = Column(Integer)
    bpm = Column(Integer)
    ecg = Column(ARRAY(Integer))

    def __repr__(self):
        return f"Entry [temperature={self.temperature}, oximetry={self.oximetry}, bpm={self.bpm}, ecg={self.ecg}]"

# 5. Criar o banco de dados (tabelas)
# Esta linha cria todas as tabelas que herdam de Base, ou seja, 'usuarios' neste exemplo
# Cria se a tabela não existe no banco de dados
Base.metadata.create_all(engine)