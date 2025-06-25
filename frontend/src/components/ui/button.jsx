export function Button({ children, onClick, className = "", ...props }) {
    return (
      <button
        onClick={onClick}
        className={`px-4 py-2 bg-blue-600 text-white rounded hover:bg-blue-700 transition ${className}`}
        {...props}
      >
        {children}
      </button>
    );
  }
  