// ts/apps/backend/src/database/db.ts

import sqlite3 from 'sqlite3';
import fs from 'fs';
import path from 'path';

let db: sqlite3.Database | null = null;
let initializationPromise: Promise<sqlite3.Database> | null = null;

// Use in-memory database for tests, otherwise use file
const dbPath = process.env.NODE_ENV === 'test'
    ? ':memory:'
    : process.env.DATABASE_URL
    ? process.env.DATABASE_URL.replace('file:', '')
    : path.resolve(__dirname, 'user.db');

if (process.env.NODE_ENV !== 'test') {
    const dbDir = path.dirname(dbPath);
    if (!fs.existsSync(dbDir)) {
        fs.mkdirSync(dbDir, { recursive: true });
    }
}

console.log('Database will be created at:', dbPath);

function query(db: sqlite3.Database, sql: string, params: any[] = []): Promise<any[]> {
    return new Promise((resolve, reject) => {
        db.all(sql, params, (err, rows) => {
            if (err) return reject(err);
            resolve(rows);
        });
    });
}

function run(db: sqlite3.Database, sql: string, params: any[] = []): Promise<sqlite3.RunResult> {
    return new Promise((resolve, reject) => {
        db.run(sql, params, function (err) {
            if (err) return reject(err);
            resolve(this);
        });
    });
}

function initDatabase(): Promise<sqlite3.Database> {
    return new Promise((resolve, reject) => {
        const dbInstance = new sqlite3.Database(dbPath, (err) => {
            if (err) {
                console.error('Error opening database:', err.message);
                return reject(err);
            }
            console.log('Connected to the SQLite database.');
            resolve(dbInstance);
        });
    });
}

async function runMigrations(db: sqlite3.Database): Promise<void> {
    console.log('=== Starting runMigrations ===');

    // TEMPORARY: Force clean slate by dropping schema_migrations table
    try {
        await run(db, 'DROP TABLE IF EXISTS schema_migrations');
        console.log('Dropped existing schema_migrations table');
    } catch (error) {
        console.log('No existing schema_migrations table to drop');
    }

    await run(db, `
        CREATE TABLE IF NOT EXISTS schema_migrations (
            version TEXT PRIMARY KEY
        )
    `);
    console.log('schema_migrations table created');

    const executedMigrations = (await query(db, 'SELECT version FROM schema_migrations'))
        .map(row => row.version);
    console.log('Executed migrations:', executedMigrations);

    // const migrationDir = path.resolve(__dirname, 'migrations');
    const migrationDir = path.resolve('/usr/src/app/apps/backend/migrations');
	console.log('Migration directory:', migrationDir);
    console.log('Directory exists:', fs.existsSync(migrationDir));

    if (fs.existsSync(migrationDir)) {
        const files = fs.readdirSync(migrationDir);
        console.log('Files in migration directory:', files);
    }

    const allMigrationFiles = fs.readdirSync(migrationDir)
        .filter(file => file.endsWith('.sql'))
        .sort();
    console.log('SQL files found:', allMigrationFiles);

    const pendingMigrations = allMigrationFiles.filter(file => !executedMigrations.includes(file));
    console.log('Pending migrations:', pendingMigrations);

    if (pendingMigrations.length === 0) {
        console.log('Database is up to date. No new migrations to run.');
        return;
    }

    console.log('Starting database migrations...');
    for (const file of pendingMigrations) {
        try {
            const filePath = path.join(migrationDir, file);
            let sql = fs.readFileSync(filePath, 'utf8');

            console.log(`Executing migration ${file}...`);
            console.log(`SQL content: ${sql.substring(0, 10000000000)}...`);

            await run(db, 'BEGIN TRANSACTION;');
            await run(db, sql);
            await run(db, 'INSERT INTO schema_migrations (version) VALUES (?)', [file]);
            await run(db, 'COMMIT;');

            console.log(`Migration ${file} executed successfully.`);
        } catch (err: any) {
            console.error(`Error executing migration ${file}:`, err.message);
            await run(db, 'ROLLBACK;');
            throw new Error(`Migration failed for ${file}`);
        }
    }
    console.log('All new migrations executed successfully.');
}

export function initializeDatabase(): Promise<sqlite3.Database> {
    if (db) {
        return Promise.resolve(db);
    }
    if (initializationPromise) {
        return initializationPromise;
    }

    initializationPromise = new Promise(async (resolve, reject) => {
        try {
            console.log('Initializing database connection...');
            const dbInstance = await initDatabase();
            await runMigrations(dbInstance);
            db = dbInstance;
            console.log('Database initialization complete');
            resolve(db);
        } catch (error) {
            console.error('Failed to initialize the database:', error);
            initializationPromise = null;
            reject(error);
            process.exit(1);
        }
    });

    return initializationPromise;
}

export async function getDatabase(): Promise<sqlite3.Database> {
    if (!db) {
        return initializeDatabase();
    }
    return db;
}
