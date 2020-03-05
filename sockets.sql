--
-- PostgreSQL database dump
--

-- Dumped from database version 10.10
-- Dumped by pg_dump version 10.10

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


--
-- Name: pgcrypto; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS pgcrypto WITH SCHEMA public;


--
-- Name: EXTENSION pgcrypto; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION pgcrypto IS 'cryptographic functions';


SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: productos; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.productos (
    id_producto integer NOT NULL,
    nombre_producto character(20),
    fecha_inicio date,
    estado_producto character varying(20),
    tiempo_transcurrido timestamp(6) with time zone,
    recurso character varying(30),
    id_usuario integer
);


ALTER TABLE public.productos OWNER TO postgres;

--
-- Name: Productos_idProducto_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public."Productos_idProducto_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public."Productos_idProducto_seq" OWNER TO postgres;

--
-- Name: Productos_idProducto_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public."Productos_idProducto_seq" OWNED BY public.productos.id_producto;


--
-- Name: productos_estado_prpoducto_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.productos_estado_prpoducto_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.productos_estado_prpoducto_seq OWNER TO postgres;

--
-- Name: productos_estado_prpoducto_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.productos_estado_prpoducto_seq OWNED BY public.productos.estado_producto;


--
-- Name: usuarios; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.usuarios (
    id_usuario integer NOT NULL,
    nombre_usuario character varying(40) NOT NULL,
    avisos integer DEFAULT 0,
    correo character varying(30),
    contrasena character varying(512)
);


ALTER TABLE public.usuarios OWNER TO postgres;

--
-- Name: usuarios_id_usuario_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.usuarios_id_usuario_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.usuarios_id_usuario_seq OWNER TO postgres;

--
-- Name: usuarios_id_usuario_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.usuarios_id_usuario_seq OWNED BY public.usuarios.id_usuario;


--
-- Name: productos id_producto; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.productos ALTER COLUMN id_producto SET DEFAULT nextval('public."Productos_idProducto_seq"'::regclass);


--
-- Name: usuarios id_usuario; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.usuarios ALTER COLUMN id_usuario SET DEFAULT nextval('public.usuarios_id_usuario_seq'::regclass);


--
-- Data for Name: productos; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.productos VALUES (1, 'Pollo               ', NULL, NULL, NULL, 'www.amazon.es', NULL);
INSERT INTO public.productos VALUES (2, 'Patatas             ', NULL, 'Procesado', NULL, 'www.google', NULL);
INSERT INTO public.productos VALUES (3, 'Pollo               ', NULL, 'Terminado', NULL, 'amazon', NULL);
INSERT INTO public.productos VALUES (4, 'Polo                ', NULL, 'Terimnado', NULL, 'noob', NULL);


--
-- Data for Name: usuarios; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.usuarios VALUES (1, 'Paco', 0, 'paco@edivb.es', '\303\015\004\007\003\002\276R\332%8\242\274ov\3225\001\377\250\276\204%\272kM\014\177\013\035x\277&`\230\007Z\237\235\256(r\234\216\250\262Y\216\334\352j\324$5\357\314\177&\334\232-\204\355#\024\373\025<?\223');
INSERT INTO public.usuarios VALUES (3, 'Miki', 0, 'hola', '1234');
INSERT INTO public.usuarios VALUES (19, 'Miserver', 0, 'hola.com', '1234');
INSERT INTO public.usuarios VALUES (25, 'Pepe', 0, 'pepe@ono', 'pepe');
INSERT INTO public.usuarios VALUES (27, 'Pepito', 0, 'pepe@manolo', '1234');
INSERT INTO public.usuarios VALUES (28, 'Hola', 0, 'Hola@mail', 'hola');
INSERT INTO public.usuarios VALUES (29, 'usuario', 0, 'usuario@usu', 'usuario');
INSERT INTO public.usuarios VALUES (30, 'ok', 0, 'ok@ok', 'ok');
INSERT INTO public.usuarios VALUES (31, 'localhost', 0, 'localhost@local', 'localhost');
INSERT INTO public.usuarios VALUES (35, 'usuario', 0, 'usuario', 'usuario');


--
-- Name: Productos_idProducto_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Productos_idProducto_seq"', 4, true);


--
-- Name: productos_estado_prpoducto_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.productos_estado_prpoducto_seq', 1, false);


--
-- Name: usuarios_id_usuario_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.usuarios_id_usuario_seq', 35, true);


--
-- Name: productos Productos_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.productos
    ADD CONSTRAINT "Productos_pkey" PRIMARY KEY (id_producto);


--
-- Name: usuarios usuarios_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.usuarios
    ADD CONSTRAINT usuarios_pkey PRIMARY KEY (id_usuario);


--
-- Name: productos fk1_id_usuario; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.productos
    ADD CONSTRAINT fk1_id_usuario FOREIGN KEY (id_usuario) REFERENCES public.usuarios(id_usuario) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--

