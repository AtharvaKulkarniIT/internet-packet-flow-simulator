export function Input({ value, onChange, placeholder, className = "", ...props }) {
    return (
      <input
        value={value}
        onChange={onChange}
        placeholder={placeholder}
        className={`border border-gray-300 px-3 py-2 rounded w-full ${className}`}
        {...props}
      />
    );
  }
  