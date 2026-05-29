#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <cstring>
# include <cerrno>


# define DEBUG 1
# define BUFFER_SIZE 4096
# define MAX_EVENTS 64
# define TIMEOUT 10000 // ms
# define CONNECTION_TIMEOUT 30 // seconds

# define PRINT_DEBUG(x) do { if (DEBUG) {std::cout << x << std::endl;} } while (0)

# define PRINT_ERR(x) do { std::cerr << x << std::endl; } while (0)

# define PRINT_PERR(x) do { PRINT_ERR(x << ": " << std::strerror(errno)); } while (0)

# define PRINT_IF_ERR(x, ERR_MSG) do { if (x < 0) { PRINT_ERR(ERR_MSG); } } while (0)

# define PRINT_IF_PERR(x, ERR_MSG) do { if (x < 0) { PRINT_PERR(ERR_MSG); } } while (0)

# define RET_ERR(ERR_MSG) do { PRINT_ERR(ERR_MSG); return -1; } while (0)

# define RET_PERR(ERR_MSG) do { PRINT_PERR(ERR_MSG); return -1; } while (0)

# define RET_IF_ERR(x, ERR_MSG) do { if (x < 0) { RET_ERR(ERR_MSG); } } while (0)

# define RET_IF_PERR(x, ERR_MSG) do { if (x < 0) { RET_PERR(ERR_MSG); } } while (0)

# define RET_IF_NULL(x, ERR_MSG) do { if (x == NULL) { RET_ERR(ERR_MSG); } } while (0)

#endif 