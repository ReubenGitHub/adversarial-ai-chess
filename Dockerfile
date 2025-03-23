FROM gcc:14.2.0

WORKDIR /app/lib
RUN wget https://github.com/Disservin/chess-library/raw/master/include/chess.hpp
WORKDIR /app

CMD ["sh", "-c", "bash"]
